#include "sdk_ps2.h"
#include "common.h"
#include "eeregs.h"
#include "snd_command_packet.h"
#include "mnu_voice_resource_path_api.h"
#include "ee_mmi.h"
#include "sdf_resource.h"
#include "sdf_packed_resource.h"
#include "sdf_pac_reloc.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "sdf_movie_stream.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"
#include "sdf_gs_packet.h"
#include "sdf_texture_lists.h"
#include "sdf_draw.h"
#include "sdf_resource_list_item.h"
#include "sdf_stream_read.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"
#include "mdl_object_stream.h"
#include "sdf_texture_offset_list.h"
#include "sdf_texture_file.h"
#include "sdf_texture_queue.h"
#include "sdf_stream_input_dma.h"
#include "sdf_sound_rpc_records.h"
#include "snd_midi_state_views.h"
#include "sdf_handler_node.h"

#define SDF_STREAM_NODE_BYTES 0x8C
#define SDF_STREAM_SCRATCH_BYTES 0x10100
#define SDF_STREAM_PREFIX_BYTES 0x100
#define SDF_STREAM_SLOT_BYTES 0x2000
#define SDF_STREAM_SLOT_SHIFT 13
#define SDF_STREAM_RING_SLOTS 8
#define SDF_STREAM_MAX_FILLED 7
#define SDF_STREAM_TRAILER_START 0x1F00
#define SDF_STREAM_QWORD_BYTES 16
#define SDF_STREAM_PIXEL_BYTES_WIDE 4
#define SDF_STREAM_PIXEL_BYTES_NARROW 2
#define SDF_STREAM_PAGE_WIDTH 64
#define SDF_STREAM_PAGE_WIDTH_MASK 63
#define SDF_STREAM_WIDE_PAGE_HEIGHT 32
#define SDF_STREAM_WIDE_HEIGHT_MASK 31
#define SDF_STREAM_NARROW_PAGE_HEIGHT 64
#define SDF_STREAM_NARROW_HEIGHT_MASK 63
#define SDF_STREAM_PSMCT32 0
#define SDF_STREAM_PSMCT16 2
#define SDF_EE_PHYSICAL_MASK 0x0FFFFFFF
#define SDF_EE_UNCACHED_BASE 0x20000000
#define SDF_IPU_DMA_START 0x100
#define SDF_IPU_DMA_NORMAL_START 0x101
#define SDF_IPU_DMA_CHAIN_START 0x105
#define SDF_IPU_FDEC 0x40000000
#define SDF_IPU_FDEC_BYTE 0x40000008

extern s32 sdfMovieDrawLeft;

extern s32 sdfMovieDrawTop;

extern s32 sdfMovieDrawRight;

extern s32 sdfMovieDrawBottom;

extern u32 sdfMovieDrawColor;

extern u32 D_003BD61C;


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

extern void sdfIpuExecQueue(s32 skipInterruptGuard);
extern s32 sdfIpuDecode(sdfIpuDecodeCb_t *);

typedef void *(*SdfSoundReadCallback)(s32);
typedef union SdfSoundCommand {
    u32 word;
    SdfSoundReadCallback read;
} SdfSoundCommand;

extern SdfSoundCommand D_003BD494;


extern sdfIpuDecodeCb_t *sdfIpuTaskLink;
extern sdfIpuDecodeCb_t *sdfIpuInputStream;
extern s32 D_003BDA94;
extern s32 D_003BDAB4;
extern void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *node, SoundFormat *format, u8 *sourceAddress, s32 sourceSize);
extern SdfHandlerNode *D_003BDA9C;
extern SdfHandlerNode *D_003BDAA0;
extern void sdfIpuRemoveHandler(SdfHandlerNode *target);

extern DevRequest *sdfTexListNew(u32 capacity);


extern sdfIpuDecodeCb_t *sdfIpuQueueHead;

extern sdfIpuDecodeCb_t *sdfIpuQueueTail;


extern void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *node);

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
    if (p2sdPcmState != 0) {
        return 1;
    }
    strcpy(D_00398948, name);
    sdfSndSendSyncCmd((channel >> 3) | 0xF0, 0, 0, 0);
    return 0;
}

u32 p2sdGetPcmState(void) {
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

u32 p2sdSetPcmCallback(u32 command) {
    if (p2sdPcmBusy != 0) {
        return 0;
    }
    D_003BD494.word = command;
    return command;
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

s32 sdfFileRpcCallback(s32 unused, u32 event) {
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
extern void sdfSndTransToIOP(s32, const void *, s32);

/* Handle sound-file/buffer RPC commands and return the shared reply words. */
u32 *sdfFileRpcHandleCmd(u32 command, SdfSoundRpcRequest *request) {
    char path[0x100];

    switch (command) {
    case 0: {
        sdfFileCmdHandle_t *state;
        if (D_00398948[0] == 0) {
            D_003FEAC0[0] = 0;
            break;
        }
        p2sdPcmState = 1;
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
    case 1: {
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
    case 2:
        sdfFileCmdClose(D_003BDA7C);
        WaitSema(sifRpcServerSema);
        sdfFileCmdFree(D_003BDA7C);
        D_003BDA7C = NULL;
        p2sdPcmState = 0;
        D_003FEAC0[0] = 0;
        break;
    case 3:
        D_003BDA78 = 0;
        D_003FEAC0[0] = 0;
        break;
    case 4: {
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
    case 6: {
        u32 word = D_003BD494.word;
        if (word != 0) {
            p2sdPcmBusy = 1;
        }
        D_003FEAC0[0] = word;
        break;
    }
    case 7: {
        void *buffer;
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
    case 8:
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

extern s32 D_003BDA8C;
extern u8 sdfSndMap[];

void sdfSoundSetTableEntry(u32 kind, u8 *src) {
    u8 *table;
    u8 *dst;
    s32 i;

    switch (kind) {
    case 0:
        table = sdfSndMap;
        dst = table + 0x210;
        for (i = 0; i < 0x20; i++) {
            *dst++ = src[i];
        }
        break;
    case 1:
        table = sdfSndMap;
        dst = table + 0x290;
        for (i = 0; i < 0x80; i++) {
            *dst++ = src[i];
        }
        break;
    case 2:
        table = sdfSndMap;
        dst = table + 0x310;
        for (i = 0; i < 0x80; i++) {
            *dst++ = src[i];
        }
        break;
    case 3:
        table = sdfSndMap;
        dst = table + 0x390;
        for (i = 0; i < 0x20; i++) {
            *dst++ = src[i];
        }
        break;
    case 4:
        table = sdfSndMap;
        dst = table + 0x410;
        for (i = 0; i < 0x20; i++) {
            *dst++ = src[i];
        }
        break;
    case 5:
        table = sdfSndMap;
        dst = table + 0x490;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 6:
        table = sdfSndMap;
        dst = table + 0x510;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 7:
        table = sdfSndMap;
        dst = table + 0x590;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 8:
        table = sdfSndMap;
        dst = table + 0x610;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 9:
        table = sdfSndMap;
        dst = table + 0x690;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 10:
        table = sdfSndMap;
        dst = table + 0x710;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 11:
        table = sdfSndMap;
        dst = table + 0x790;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 12:
        table = sdfSndMap;
        dst = table + 0x810;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    }
    sdfSndTransToIOP(D_003BDA8C, sdfSndMap, 0x8D0);
    sdfSndSendSyncCmd(0x1F0, 0, 0, 0);
}

void sdfSoundGetTableEntry(u32 kind, u8 *dst) {
    u8 *table;
    u8 *src;
    s32 i;

    sdfSndSendSyncCmd(0x200, 0, 0, 0);
    switch (kind) {
    case 0:
        table = sdfSndMap;
        src = table + 0x210;
        for (i = 0; i < 0x20; i++) {
            dst[i] = *src++;
        }
        return;
    case 1:
        table = sdfSndMap;
        src = table + 0x290;
        for (i = 0; i < 0x80; i++) {
            dst[i] = *src++;
        }
        return;
    case 2:
        table = sdfSndMap;
        src = table + 0x310;
        for (i = 0; i < 0x80; i++) {
            dst[i] = *src++;
        }
        return;
    case 3:
        table = sdfSndMap;
        src = table + 0x390;
        for (i = 0; i < 0x20; i++) {
            dst[i] = *src++;
        }
        return;
    case 4:
        table = sdfSndMap;
        src = table + 0x410;
        for (i = 0; i < 0x20; i++) {
            dst[i] = *src++;
        }
        return;
    case 5:
        table = sdfSndMap;
        src = table + 0x490;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 6:
        table = sdfSndMap;
        src = table + 0x510;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 7:
        table = sdfSndMap;
        src = table + 0x590;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 8:
        table = sdfSndMap;
        src = table + 0x610;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 9:
        table = sdfSndMap;
        src = table + 0x690;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 10:
        table = sdfSndMap;
        src = table + 0x710;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 11:
        table = sdfSndMap;
        src = table + 0x790;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 12:
        table = sdfSndMap;
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
INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B4880);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48A0);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48B8);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48E0);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B4900);

void sdfDumpDmaChain(u32 *src) {
    u32 *callStack[2];
    u32 *cursor;
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
        tag = cursor[0];
        tagAddress = cursor[1];
        tagType = (tag >> 28) & 7;
        tagCount = tag & 0xFFFF;
        sdfPrintf(D_003B48B8, cursor, D_00398968[tagType], tag, tagAddress, cursor[2], cursor[3]);
        cursor += 4;
        switch (tagType) {
        case 0:
            done = 1;
            break;
        case 1:
            cursor += tagCount * 4;
            break;
        case 2:
            cursor = (u32 *)tagAddress;
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
            cursor = (u32 *)tagAddress;
            break;
        case 6:
            if (depth == 0) {
                done = 1;
            } else {
                cursor = callStack[--depth];
            }
            break;
        case 7:
            cursor += tagCount * 4;
            done = 1;
            break;
        }
        if (done != 0) {
            sdfPrintf(D_003B4900, cursor);
            return;
        }
    }
}




extern char D_003BD508[];
extern char D_003BD518[];
extern char D_003BD528[];
extern char D_003BD530[];
extern char D_003BD5E0[];
extern char D_003BD5E8[];
extern char *D_00398988[16];
extern char *D_003989C8[8];
extern s32 D_003989E8[16];

void sdfGrFlushOtChain(u32 sourceAddress) {
    u32 *returnStack[2];
    u32 *cursor;
    u32 *wordCursor = NULL;
    s32 dataMode = 0;
    s32 tagId = 0;
    s32 depth = 0;
    s32 n = 0;
    s32 directRow;
    s32 phase;
    s32 wordsLeft;
    u32 tagAddress;
    s32 qwc;
    s32 done;
    u32 value;

    sdfPrintf("<<< DMA-VIF dump start [%p] >>>\n", (void *)sourceAddress);
    cursor = (u32 *)sourceAddress;
    directRow = 0;
    tagAddress = 0;
    qwc = 0;
    phase = 0;
    wordsLeft = 0;
    done = 0;
    for (;;) {
        if (wordsLeft == 0) {
            switch (phase) {
            case 0: {
                u32 tag;
                if ((u32)cursor & 0xF) {
                    sdfPrintf(D_003B48A0, cursor);
                    return;
                }
                tag = cursor[0];
                tagAddress = cursor[1];
                tagId = (tag >> 28) & 7;
                qwc = tag & 0xFFFF;
                sdfPrintf("\n%08X [DMA %s] : %08X %08X\n",
                                           cursor, D_00398968[tagId], tag, tagAddress);
                wordCursor = cursor + 2;
                cursor += 4;
                wordsLeft = 2;
                phase = 1;
                break;
            }
            case 1:
                wordsLeft = qwc * 4;
                switch (tagId) {
                case 0:
                case 3:
                case 4:
                    wordCursor = (u32 *)tagAddress;
                    break;
                default:
                    wordCursor = cursor;
                    break;
                }
                phase = 2;
                break;
            case 2:
                switch (tagId) {
                case 0:
                case 7:
                    done = 1;
                    break;
                case 1:
                    cursor += qwc * 4;
                    break;
                case 2:
                    cursor = (u32 *)tagAddress;
                    break;
                case 3:
                case 4:
                    break;
                case 5:
                    if (depth == 2) {
                        sdfPrintf("\ncall stack nest overflow!\n");
                        return;
                    }
                    returnStack[depth++] = cursor + qwc * 4;
                    cursor = (u32 *)tagAddress;
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
                phase = 0;
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

INCLUDE_RODATA(const s32, "game/code_002E9708", sdfGsMemoryDumpHeader);

INCLUDE_RODATA(const s32, "game/code_002E9708", sdfGsMemoryDumpRowFormat);

void sdfDumpMemory(void) {
    SdfGeneralHeapStats info;
    sdfMemGetInfo(&info);
    sdfPrintf(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info.totalBytes, info.freeBytes,
                    info.largestFreeBytes, info.smallestFreeBytes,
                    info.blockCount, info.freeBlockCount);
}

extern char D_003BD518[];

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


/* Read a named file through the dev RPC into a freshly allocated block; returns the block's handle.
 * outData receives the block address, outSize the file size; without outData the block is released. */
sdfMemHandle_t *sdfDevLoadFile(const char *name, u32 *outData, u32 *outSize, s32 extra) {
    sdfFileCmdHandle_t *state = sdfFileOpen(name);
    s32 size = sdfFileLength(state);
    sdfMemHandle_t *handle = sdfMemAllocDirect(size + extra);
    void *data = sdfMemLock(handle);

    sdfFileRead(state, data, size);
    sdfFileClose(state);
    if (outData != NULL) {
        *outData = (u32)data;
    } else {
        sdfMemUnlock(handle);
    }
    if (outSize != NULL) {
        *outSize = size;
    }
    return handle;
}

sdfMemHandle_t *sdfDevLoadFileEx(const char *name, u32 *outAddress, u32 *outSize) {
    return sdfDevLoadFile(name, outAddress, outSize, 0);
}

sdfTexHandle_t *sdfLoadNamedResourceAndReleaseLookupHandle(const char *name) {
    sdfMemHandle_t *handle;
    sdfTexHandle_t *resource;
    u32 info[4];

    handle = sdfDevLoadFileEx(name, info, 0);
    resource = sdfTexSet((SdfTextureFileHeader *)(info[0]));
    sdfMemFreeDirect(handle);
    return resource;
}

DevRequest *sdfTexSetTB(const SdfTextureOffsetListHeader *resource) {
    s32 i = 0;
    s32 count = resource->textureCount;
    DevRequest *handle = sdfTexListNew(count);
    const SdfTextureOffsetListEntry *entry;
    if (count != i) {
        entry = (const SdfTextureOffsetListEntry *)((const u8 *)resource + sizeof(*resource));
        do {
            i++;
            sdfTexListAdd(handle, sdfTexSet((SdfTextureFileHeader *)(
                (void *)((const u8 *)resource + entry->textureHeaderOffset))));
            entry++;
        } while (i != count);
    }
    return handle;
}

DevRequest *sndLoadNamedOffsetResourceList(const char *name) {
    sdfMemHandle_t *handle;
    DevRequest *resource;
    u32 info[4];

    handle = sdfDevLoadFileEx(name, info, 0);
    resource = sdfTexSetTB((const SdfTextureOffsetListHeader *)(u32)info[0]);
    sdfMemFreeDirect(handle);
    return resource;
}

/* Relocate the payload following the packed header and return its address. */
u8 *sdfRelocatePackedResourcePayload(SdfPackedRelocationHeader *resource) {
    u8 *payload = (u8 *)resource + SDF_PACKED_RESOURCE_HEADER_BYTES;

    sdfAddressResolver((s32 *)payload, (s32)payload,
                                   payload + resource->relocationOffset, resource->relocationByteCount);
    return payload;
}

/* Return the retained allocation handle and publish the relocated payload address. */
sdfMemHandle_t *sdfLoadPackedResourceWithRelocatedPayload(const char *name, u8 **outPayload) {
    u32 info[4];
    sdfMemHandle_t *buffer = sdfDevLoadFileEx(name, info, 0);
    *outPayload = sdfRelocatePackedResourcePayload((SdfPackedRelocationHeader *)(u32)info[0]);
    return buffer;
}

/* This entry point uses the same payload-relative relocation table. */
u8 *sdfRelocatePackedResourceWordsFromHeader(SdfPackedRelocationHeader *resource) {
    u8 *payload = (u8 *)resource + SDF_PACKED_RESOURCE_HEADER_BYTES;

    sdfAddressResolver((s32 *)payload, (s32)payload,
                                   payload + resource->relocationOffset, resource->relocationByteCount);
    return payload;
}

/* Return the retained allocation handle and publish the relocated payload address. */
sdfMemHandle_t *sdfReadPackedResourceAndRelocateHeader(const char *name, u8 **outPayload) {
    u32 info[4];
    sdfMemHandle_t *buffer = sdfDevLoadFileEx(name, info, 0);
    *outPayload = sdfRelocatePackedResourceWordsFromHeader((SdfPackedRelocationHeader *)(u32)info[0]);
    return buffer;
}

/* Byte-coded commands advance by word deltas, or relocate a run of following words.
 * Low tag bits select one-, two-, or three-byte deltas; tag 7 encodes a run.
 * tableBytes bounds the encoded byte stream, not the number of relocated words.
 */
void sdfAddressResolver(s32 *words, s32 relocationBase, const u8 *table, s32 tableBytes) {
    const u8 *commandCursor = table;
    s32 value;
    s32 runIndex;

    while (commandCursor - table < tableBytes) {
        value = *commandCursor++;
        if ((value & 1) == 0) {
            value >>= 1;
        } else if ((value & 2) == 0) {
            value = (value | *commandCursor++ << 8) >> 2;
        } else if ((value & 4) == 0) {
            value = (value | commandCursor[0] << 8 | commandCursor[1] << 16) >> 3;
            commandCursor += 2;
        } else {
            value = (value >> 3) + 2;
            for (runIndex = 0; runIndex < value; runIndex++) {
                words++;
                *words += relocationBase;
            }
            continue;
        }
        words += value;
        *words += relocationBase;
    }
}

/* Remove a queued node without clearing its old links; the caller may own the interrupt guard. */
void sdfIpuDetachQueue(sdfIpuDecodeCb_t *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    sdfIpuDecodeCb_t *previousNode;
    sdfIpuDecodeCb_t *nextNode;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = DIntr();
    }
    if (node->queued != 0) {
        previousNode = node->streamPrev;
        nextNode = node->streamNext;
        if (previousNode == 0) {
            sdfIpuQueueHead = nextNode;
        } else {
            previousNode->streamNext = nextNode;
        }
        if (nextNode == 0) {
            sdfIpuQueueTail = previousNode;
        } else {
            nextNode->streamPrev = previousNode;
        }
        node->queued = 0;
    }
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Move a node to the queue tail, using one interrupt guard for unlink plus append. */
void sdfIpuAttachQueue(sdfIpuDecodeCb_t *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = DIntr();
    }
    if (node->queued != 0) {
        sdfIpuDetachQueue(node, 1);
    }
    node->queued = 1;
    if (sdfIpuQueueTail == 0) {
        sdfIpuQueueHead = node;
    } else {
        sdfIpuQueueTail->streamNext = node;
    }
    node->streamPrev = sdfIpuQueueTail;
    node->streamNext = 0;
    sdfIpuQueueTail = node;
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Walk link addresses to append to the independent singly-linked sound list. */
void sdfIpuAttachTask(sdfIpuDecodeCb_t *node) {
    sdfIpuDecodeCb_t **tailLink = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *currentNode = *tailLink;
    if (currentNode != NULL) {
        tailLink = &currentNode->next;
        while ((currentNode = *tailLink) != NULL) {
            tailLink = &currentNode->next;
        }
    }
    *tailLink = node;
    node->next = NULL;
}

/* Unlink the first matching sound node; absence leaves the list unchanged. */
void sdfIpuDetachTask(sdfIpuDecodeCb_t *node) {
    sdfIpuDecodeCb_t **nodeLink = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *currentNode = *nodeLink;
    while (currentNode != NULL) {
        if (currentNode == node) {
            *nodeLink = currentNode->next;
            return;
        }
        nodeLink = &currentNode->next;
        currentNode = *nodeLink;
    }
}

/* Allocate two width-by-height frames, using four bytes per pixel for mode zero, two otherwise. */
void sdfIpuAllocBuff(sdfIpuDecodeCb_t *node) {
    s32 bytesPerPixel = SDF_STREAM_PIXEL_BYTES_WIDE;
    s32 size;
    if (node->audioMode != SDF_STREAM_AUDIO_DISABLED) {
        bytesPerPixel = SDF_STREAM_PIXEL_BYTES_NARROW;
    }
    size = node->width * node->height;
    size *= bytesPerPixel;
    node->bufferSize = size;
    node->frameBuffers[0] = sdfAllocAuto(size);
    node->frameBuffers[1] = sdfAllocAuto(size);
}

/* Each 16-byte DMA tag holds control/address bits and a zero reserved half. */
void sdfIpuMakeToIpuDmaTag(sdfIpuDecodeCb_t *node, u8 *source, s32 bytes) {
    s32 remaining = (bytes + 15) & ~15;
    SdfStreamInputDmaTag *tag;
    u32 address;

    tag = sdfAllocAuto(((remaining + 0xFFEFF) / 0xFFF00) * 16);
    node->inputDmaChain = tag;
    address = (u32)source & SDF_EE_PHYSICAL_MASK;
    do {
        s32 chunk = remaining > 0xFFF00 ? 0xFFF00 : remaining;
        u32 id;
        remaining -= chunk;
        id = remaining != 0 ? 3 : 0;
        tag->control = ((u16)(chunk >> 4) | (id << 28)) | ((u64)address << 32);
        tag->reserved = 0;
        address += chunk;
        tag++;
    } while (remaining > 0);
    FlushCache(0);
}

/* Build the two GIF upload buffers for the node's tiled frame images. */
void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *node) {
    s32 pixelMode;
    u32 tileQwords;
    s32 width;
    s32 height;
    s32 bufferWidth;
    s32 remainingTiles;
    s32 packetBytes;
    s32 sourceStride;
    s32 bufferIndex;
    s32 x;
    s32 y;
    s32 endOfPacket;
    u32 sourceAddress;
    u64 bitblt;
    u32 sourceTag;
    u64 imageTag;
    u64 *packet;

    if (node->audioMode == SDF_STREAM_AUDIO_DISABLED) {
        pixelMode = SDF_STREAM_PSMCT32;
        tileQwords = 64;
    } else {
        pixelMode = SDF_STREAM_PSMCT16;
        tileQwords = 32;
    }
    width = node->width;
    height = node->height;
    /* Retail shares this count across both packet buffers. */
    remainingTiles = (node->width >> 4) * (node->height >> 4);
    bufferWidth = width >> 6;
    if (bufferWidth == 0) {
        bufferWidth = 1;
    }
    bitblt = ((u64)pixelMode << 56) | ((u64)bufferWidth << 48);
    sourceTag = tileQwords | 0x30000000;
    imageTag = (u64)(tileQwords | 0x50000000) << 32;
    sourceStride = tileQwords << 4;
    packetBytes = remainingTiles * 0x60 + 0x50;
    node->transferPacketBytes = packetBytes;

    bufferIndex = 0;
    do {
        packet = sdfAllocAuto(packetBytes);
        node->transferPacketBuffers[bufferIndex] = packet;
        sourceAddress = (u32)node->frameBuffers[bufferIndex] & SDF_EE_PHYSICAL_MASK;

        packet[0] = 0x10000003;
        packet[1] = (u64)0x50000003 << 32;
        packet[4] = bitblt | ((u64)((u32)node->resourceWord >> 6) << 32);
        packet[5] = 0x50;
        packet[6] = ((u64)16 << 32) | 16;
        packet[7] = 0x52;
        packet[2] = ((u64)0x10000000 << 32) | 2;
        packet[3] = 0xE;
        packet += 8;

        endOfPacket = 0;
        y = 0;
        do {
            x = 0;
            do {
                remainingTiles--;
                if (remainingTiles == 0) {
                    endOfPacket = 1;
                }
                packet[0] = 0x10000004;
                packet[1] = (u64)0x50000004 << 32;
                packet[5] = 0x51;
                packet[4] = ((u64)y << 48) | ((u64)x << 32);
                packet[8] = (u64)(endOfPacket << 15) | (u64)tileQwords | ((u64)2 << 58);
                packet[10] = sourceTag | ((u64)sourceAddress << 32);
                sourceAddress += sourceStride;
                packet[2] = ((u64)0x10000000 << 32) | 2;
                packet[3] = 0xE;
                packet[6] = 0;
                packet[7] = 0x53;
                packet[9] = 0;
                packet[11] = imageTag;
                x += 16;
                packet += 12;
            } while (x < width);
            y += 16;
        } while (y < height);
        bufferIndex++;
    } while (bufferIndex != 2);
}

/* Clear the node, map the format's two selector bytes to mode 0/1/2, and copy loop/playback modes. */
void sdfIpuInitCb(sdfIpuDecodeCb_t *node, SoundFormat *format) {
    memset(node, 0, SDF_STREAM_NODE_BYTES);
    if (format->hasAudio == 0) {
        node->audioMode = SDF_STREAM_AUDIO_DISABLED;
    } else if (format->stereo == 0) {
        node->audioMode = SDF_STREAM_AUDIO_MONO;
    } else {
        node->audioMode = SDF_STREAM_AUDIO_STEREO;
    }
    node->loopMode = format->loopMode;
    node->playbackCadenceStep = format->playbackCadenceStep;
}
/* Decode the frame header, allocate its buffers, then queue the bytes after that header. */
void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *node, SoundFormat *format, u8 *sourceAddress, s32 sourceSize) {
    s32 interruptsEnabled;
    u8 *frameBytes = sourceAddress;

    sdfIpuInitCb(node, format);
    node->width = ((SdfStreamFrameHeader *)frameBytes)->width;
    node->cycleLength = ((SdfStreamFrameHeader *)frameBytes)->cycleLength;
    node->height = ((SdfStreamFrameHeader *)frameBytes)->height;
    sdfIpuAllocBuff(node);
    sdfIpuMakeToIpuDmaTag(node, frameBytes + SDF_STREAM_FRAME_HEADER_BYTES, sourceSize - SDF_STREAM_FRAME_HEADER_BYTES);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}

/* Store the callback and movie context, then allocate the feed ring and leading mirror bytes. */
void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *node, SoundFormat *format, SdfStreamRead readSource, movieStream_t *readContext) {
    sdfIpuInitCb(node, format);
    node->read = readSource;
    node->readContext = readContext;
    node->active = 1;
    node->scratchBuffer = (u8 *)sdfAllocAuto(SDF_STREAM_SCRATCH_BYTES) + SDF_STREAM_PREFIX_BYTES;
}

extern sdfGsMemSlice_t *sdfGsMemAllocRect(s32, s32, s32, s32, s32);

/* Query for a complete header before reading it. Texture dimensions are rounded
 * only when larger than one GS page; smaller dimensions remain unchanged.
 */
void sdfIpuAllocGsRect(sdfIpuDecodeCb_t *node) {
    SdfStreamFrameHeader header;
    s32 readStatus;
    s32 interruptsEnabled;

    if (node->headerReady != 0) {
        return;
    }
    if (node->read(node, node->readContext, SDF_STREAM_READ_QUERY, &readStatus, 0) < sizeof(header)) {
        return;
    }
    node->headerReady = 1;
    node->read(node, node->readContext, SDF_STREAM_READ_COPY, &header, sizeof(header));
    node->width = header.width;
    node->height = header.height;
    node->cycleLength = header.cycleLength;
    if (node->resourceWord == 0) {
        s32 textureWidth = node->width;
        s32 textureHeight = node->height;
        s32 pixelFormat;
        sdfGsMemSlice_t *texture;

        if (textureWidth > SDF_STREAM_PAGE_WIDTH) {
            textureWidth = (textureWidth + SDF_STREAM_PAGE_WIDTH_MASK) & ~SDF_STREAM_PAGE_WIDTH_MASK;
        }
        if (node->audioMode == SDF_STREAM_AUDIO_DISABLED) {
            pixelFormat = SDF_STREAM_PSMCT32;
            if (textureHeight > SDF_STREAM_WIDE_PAGE_HEIGHT) {
                textureHeight = (textureHeight + SDF_STREAM_WIDE_HEIGHT_MASK) & ~SDF_STREAM_WIDE_HEIGHT_MASK;
            }
        } else {
            pixelFormat = SDF_STREAM_PSMCT16;
            if (textureHeight > SDF_STREAM_NARROW_PAGE_HEIGHT) {
                textureHeight = (textureHeight + SDF_STREAM_NARROW_HEIGHT_MASK) & ~SDF_STREAM_NARROW_HEIGHT_MASK;
            }
        }
        texture = sdfGsMemAllocRect(textureWidth, textureHeight, pixelFormat, SDF_TEX_RESOURCE_TEXTURE, 0);
        node->textureHead = texture;
        node->resourceWord = texture->word;
    }
    sdfIpuAllocBuff(node);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
}

extern sdfIpuDecodeCb_t *sdfCurrIpuDecodeCb;
extern void sdfImageSetChainReleaseMem(void *);
extern void sdfGsMemFreeDelayed(sdfGsMemSlice_t *texture);

void sdfIpuFree(sdfIpuDecodeCb_t *node) {
    s32 interruptsEnabled;
    s32 wasActive;
    u32 dmaEnable;

    interruptsEnabled = DIntr();
    wasActive = 0;
    node->drained = 1;
    sdfIpuDetachTask(node);
    sdfIpuDetachQueue(node, 1);
    if (sdfCurrIpuDecodeCb == node) {
        HW_WRITE(IPU_CTRL, 0x40000000);
        sceIpuSync(0, 0);
        dmaEnable = HW_READ(D_ENABLER);
        HW_WRITE(D_ENABLER, 0x10000);
        HW_WRITE(D4_CHCR, 1);
        HW_WRITE(D3_CHCR, 0);
        HW_WRITE(D_ENABLER, dmaEnable);
        sdfCurrIpuDecodeCb = NULL;
        wasActive = 1;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfFreeAuto(node->inputDmaChain);
    sdfFreeAuto(node->frameBuffers[0]);
    sdfFreeAuto(node->frameBuffers[1]);
    sdfImageSetChainReleaseMem(node->transferPacketBuffers[0]);
    sdfImageSetChainReleaseMem(node->transferPacketBuffers[1]);
    if (node->scratchBuffer != 0) {
        sdfFreeAuto(node->scratchBuffer - 0x100);
    }
    if (node->textureHead != 0) {
        sdfGsMemFreeDelayed(node->textureHead);
    }
    if (wasActive != 0) {
        sdfIpuExecQueue(0);
    }
}


/* Fill at most seven of eight 8-KiB slots. A short non-EOF query waits for more
 * data; EOF permits a partial final read. Copy the final slot's trailing bytes
 * into the prefix mirror, retaining the original copy even for a partial read.
 */
void sdfIpuHandleDmaEnd(sdfIpuDecodeCb_t *feed) {
    s32 writeSlot;
    s32 filledSlots;
    s32 availableBytes;
    u32 destinationAddress;
    s32 endOfStream;

    filledSlots = feed->filledSlots;
    if (feed->done == 0 && filledSlots < SDF_STREAM_MAX_FILLED) {
        do {
            writeSlot = feed->firstSlot;
            endOfStream = 0;
            destinationAddress = (u32)feed->scratchBuffer;
            writeSlot += filledSlots;
            if (writeSlot >= SDF_STREAM_RING_SLOTS) {
                writeSlot -= SDF_STREAM_RING_SLOTS;
            }
            destinationAddress += writeSlot << SDF_STREAM_SLOT_SHIFT;
            destinationAddress = (destinationAddress & SDF_EE_PHYSICAL_MASK) | SDF_EE_UNCACHED_BASE;
            availableBytes = feed->read(feed, feed->readContext, SDF_STREAM_READ_QUERY, &endOfStream, 0);
            if (availableBytes < SDF_STREAM_SLOT_BYTES) {
                if (endOfStream == 0) {
                    return;
                }
                if (availableBytes > 0) {
                    feed->read(feed, feed->readContext, SDF_STREAM_READ_COPY, (void *)destinationAddress, availableBytes);
                    filledSlots++;
                }
                feed->done = 1;
            } else {
                if (availableBytes == SDF_STREAM_SLOT_BYTES) {
                    if (endOfStream != 0) {
                        feed->done = 1;
                    }
                }
                feed->read(feed, feed->readContext, SDF_STREAM_READ_COPY, (void *)destinationAddress, SDF_STREAM_SLOT_BYTES);
                filledSlots++;
            }
            if (writeSlot == SDF_STREAM_MAX_FILLED) {
                memcpy((void *)(((u32)(feed->scratchBuffer - SDF_STREAM_PREFIX_BYTES) & SDF_EE_PHYSICAL_MASK) | SDF_EE_UNCACHED_BASE), (void *)(destinationAddress + SDF_STREAM_TRAILER_START), SDF_STREAM_PREFIX_BYTES);
            }
            feed->filledSlots = filledSlots;
            if (feed->done != 0) {
                break;
            }
        } while (filledSlots < SDF_STREAM_MAX_FILLED);
    }
}

/* Start IPU input from the initial chain or a feed slot; defer an empty ring. */
void sdfIpuSetToIpuDma(sdfIpuDecodeCb_t *stream) {
    if (stream->active == 0) {
        sdfIpuInputStream = stream;
        HW_WRITE(D4_TADR, (u32)stream->inputDmaChain & SDF_EE_PHYSICAL_MASK);
        HW_WRITE(D4_QWC, 0);
        HW_WRITE(D4_CHCR, SDF_IPU_DMA_CHAIN_START);
    } else {
        if (stream->filledSlots == 0) {
            stream->inputDmaStartPending = 1;
            return;
        }
        sdfIpuInputStream = stream;
        stream->inputFeedDmaInFlight = 1;
        HW_WRITE(D4_MADR, ((u32)stream->scratchBuffer + (stream->firstSlot << SDF_STREAM_SLOT_SHIFT)) &
                              SDF_EE_PHYSICAL_MASK);
        HW_WRITE(D4_QWC, SDF_STREAM_SLOT_BYTES / SDF_STREAM_QWORD_BYTES);
        HW_WRITE(D4_CHCR, SDF_IPU_DMA_NORMAL_START);
    }
    stream->inputDmaStartPending = 0;
}

/* Start DMA from the IPU into the selected buffer, then toggle the byte index. */
void sdfIpuSetFromIpuDma(sdfIpuDecodeCb_t *stream) {
    u8 bufferIndex = stream->bufferIndex;
    HW_WRITE(D3_MADR, (u32)stream->frameBuffers[bufferIndex] & SDF_EE_PHYSICAL_MASK);
    HW_WRITE(D3_QWC, stream->bufferSize / SDF_STREAM_QWORD_BYTES);
    HW_WRITE(D3_CHCR, SDF_IPU_DMA_START);
    stream->bufferIndex = bufferIndex ^ 1;
}

/* Read the FDEC result, then issue FDEC with an eight-bit advance; return the earlier result. */
s32 IpuWriteCommand(void) {
    s32 decodedBits;
    HW_WRITE(IPU_CMD, SDF_IPU_FDEC);
    sceIpuSync(0, 0);
    decodedBits = HW_READ(IPU_CMD);
    sceIpuSync(0, 0);
    HW_WRITE(IPU_CMD, SDF_IPU_FDEC_BYTE);
    sceIpuSync(0, 0);
    return decodedBits;
}

INCLUDE_ASM(const s32, "game/code_002E9708", sdfIpuDecode);

/* Remove and advance the first list item matching the IPU cleanup test. */
void sdfIpuExecQueue(s32 skipInterruptGuard) {
    sdfIpuDecodeCb_t *node;
    s32 interruptsEnabled = 0;

    if (sdfCurrIpuDecodeCb != NULL) {
        return;
    }
    if (skipInterruptGuard == 0) {
        interruptsEnabled = DIntr();
    }

    node = sdfIpuQueueHead;
    while (node != 0) {
        if (node->completedBufferCount + node->pendingPlaybackSubmissions < 2 &&
            (node->active != 1 || node->filledSlots != 0)) {
            sdfIpuDetachQueue(node, 1);
            sdfIpuDecode(node);
            break;
        }
        node = node->streamNext;
    }

    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}



/* Sleep for IPU completion, advance cycle state and requeue non-drained nodes;
 * the DMA worker runs indefinitely and retains the native completion-byte gates. */
void sdfIpuDecodeThread(void) {
    sdfIpuDecodeCb_t *work;
    s32 interruptsEnabled;

    for (;;) {
        SleepThread();
        work = sdfCurrIpuDecodeCb;
        if (work == NULL) {
            continue;
        }
        sceIpuStopDMA(&work->dma);
        if (work->dma.inputQwords == 0) {
            work->inputFeedEnabled = 0;
        }
        if (work->playbackPhase == SDF_STREAM_PLAYBACK_INITIAL) {
            work->playbackPhase = SDF_STREAM_PLAYBACK_FIRST_COMPLETION;
        }
        work->completedBufferCount++;
        sdfCurrIpuDecodeCb = NULL;
        work->tickCount++;
        if (work->tickCount == work->cycleLength) {
            if (work->loopMode != 0) {
                work->ipuCycleInitialized = 0;
                work->tickCount = 0;
                work->unk65 = 0;
            } else {
                work->drained = 1;
            }
        }
        if (work->drained == 0) {
            interruptsEnabled = DIntr();
            sdfIpuAttachQueue(work, 0);
            if (interruptsEnabled != 0) {
                EIntr();
            }
        }
        sdfIpuExecQueue(0);
    }
}

s32 sdfIpuFromDmacHandler(void) {
    iWakeupThread(D_003BDAB4);
    EE_ENABLE_INTERRUPTS_SYNC();
    return 0;
}

s32 sdfIpuToDmacHandler(void) {
    sdfIpuDecodeCb_t *stream = sdfIpuInputStream;

    if (stream != NULL) {
        D_003BDA94 = stream->playbackFrameIndex;
        sdfIpuInputStream = NULL;
        if (stream->inputFeedEnabled != 0 && stream->active == 1) {
            if (stream->inputFeedDmaInFlight != 0) {
                stream->inputFeedDmaInFlight = 0;
                stream->firstSlot++;
                if (stream->firstSlot == SDF_STREAM_RING_SLOTS) {
                    stream->firstSlot = 0;
                }
                stream->filledSlots--;
            }
            sdfIpuHandleDmaEnd(stream);
            sdfIpuSetToIpuDma(stream);
        }
    }
    EE_ENABLE_INTERRUPTS_SYNC();
    return 0;
}

/* Return the indexed earlier entry only when enabled; the byte index is unchecked. */
u32 sdfIpuGetActivePacket(SndMidiChannel *channel) {
    u32 selectedEntry = 0;
    if (channel->isEnabled != 0) {
        selectedEntry = channel->previousEntries[channel->entryIndex];
    }
    return selectedEntry;
}

/* Return the indexed current entry only when enabled; the byte index is unchecked. */
u32 sdfIpuGetActiveFrame(SndMidiChannel *channel) {
    u32 selectedEntry = 0;
    if (channel->isEnabled != 0) {
        selectedEntry = channel->currentEntries[channel->entryIndex];
    }
    return selectedEntry;
}

/* Consume a pending buffer when present, but always advance processed; loop only on exact equality. */
void sdfIpuFinishActiveFrame(sdfIpuDecodeCb_t *node) {
    s32 interruptsEnabled = DIntr();
    s32 pendingBuffers = node->completedBufferCount;
    s32 remainingBuffers = pendingBuffers - 1;
    if (pendingBuffers > 0) {
        node->completedBufferCount = remainingBuffers;
        node->transferPacketIndex ^= 1;
        node->pendingPlaybackSubmissions++;
    }
    node->playbackFrameIndex++;
    if (node->playbackFrameIndex == node->cycleLength && node->loopMode != 0) {
        node->playbackFrameIndex = 0;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}

/* Return zero only when nothing is pending; otherwise call the zero-buffer handler if needed, queue and advance. */
s32 sdfIpuSetChainTag(sdfIpuDecodeCb_t *node) {
    u32 *selectedBuffer;
    if (node->completedBufferCount == 0) {
        return 0;
    }
    selectedBuffer = (u32 *)(node->transferPacketIndex * 4 + (s32)node + 0x28);
    if (*selectedBuffer == 0) {
        sdfIpuMakeBltPacket(node);
    }
    sdfImageSetChainTag(*selectedBuffer, (SdfTextureDmaTail *)(*selectedBuffer + node->transferPacketBytes - SDF_STREAM_QWORD_BYTES));
    sdfIpuFinishActiveFrame(node);
    return 1;
}
extern u32 D_003BD614;

/* Advance playback cadence, refill active feeds and retry a deferred IPU input. */
void sdfAdvanceStreamPlayback(s32 cadence) {
    sdfIpuDecodeCb_t *node = sdfCurrIpuDecodeCb;
    s32 elapsed;
    s32 interruptsEnabled;

    if (node != NULL && node->inputDmaStartPending != 0) {
        sdfIpuSetToIpuDma(node);
    }
    node = sdfIpuTaskLink;
    while (node != NULL) {
        if (node->pendingPlaybackSubmissions != 0) {
            node->pendingPlaybackSubmissions--;
        }
        elapsed = node->playbackCadenceRemainder;
        /* The playback provider views the same 0x8C stream allocation. */
        switch (node->playbackPhase) {
        case SDF_STREAM_PLAYBACK_FIRST_COMPLETION:
            sdfIpuSetChainTag(node);
            node->playbackPhase = SDF_STREAM_PLAYBACK_CADENCED;
            node->playbackCadenceRemainder = node->playbackCadenceStep;
            break;
        case SDF_STREAM_PLAYBACK_CADENCED:
            if (D_003BD614 != 1) {
                elapsed += node->playbackCadenceStep;
                if (elapsed >= cadence) {
                    if (sdfIpuSetChainTag(node) != 0) {
                        elapsed -= cadence;
                    }
                }
                node->playbackCadenceRemainder = elapsed;
            }
            break;
        }
        if (node->active == 1) {
            sdfIpuAllocGsRect(node);
            if (D_003BD614 != 1) {
                if (node->filledSlots < SDF_STREAM_RING_SLOTS) {
                    interruptsEnabled = DIntr();
                    sdfIpuHandleDmaEnd(node);
                    if (interruptsEnabled != 0) {
                        EIntr();
                    }
                }
                node->read(node, node->readContext, SDF_STREAM_READ_RESUME, NULL, 0);
            }
        }
        node = node->next;
    }
    sdfIpuExecQueue(0);
}

void sdfSoundInitAndAppendNode(sdfIpuDecodeCb_t *node, SoundFormat *format, u8 *source, s32 sourceSize, s32 resourceWord) {
    sdfIpuSetDecodeParams(node, format, source, sourceSize);
    node->resourceWord = resourceWord;
    sdfIpuAttachTask(node);
}

void sdfIpuSetOutputTex(sdfIpuDecodeCb_t *node, SdfStreamParams *params, u8 *sourceData, s32 sourceSize, sdfTexHandle_t *source) {
    SdfStreamParams streamParams = *params;
    switch (source->pixelFormat) {
    case 0:
        streamParams.hasAudio = 0;
        break;
    case 2:
        streamParams.hasAudio = 1;
        break;
    }
    sdfSoundInitAndAppendNode(node, &streamParams, sourceData, sourceSize, sdfTexGetPixelAddr(source));
}

void sdfSoundInitFormattedAndAppendNode(sdfIpuDecodeCb_t *node, SoundFormat *format, SdfStreamRead read, movieStream_t *readContext, s32 resourceWord) {
    sdfIpuStrmDecodeInit(node, format, read, readContext);
    node->resourceWord = resourceWord;
    sdfIpuAttachTask(node);
}

extern SdfHandlerNode *sdfAddHandler(SdfHandlerKind, s32, SdfHandlerCallback, s32, s32);
extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);
extern u8 sdfIpuDecodeThStack[];
extern void sdfIpuDecodeThread();
void sdfIpuInit(void) {
    s32 thread;

    sceIpuInit();
    HW_WRITE(IPU_CMD, 0x90000000);
    sdfIpuTaskLink = 0;
    sdfCurrIpuDecodeCb = NULL;
    D_003BDA9C = sdfAddHandler(SDF_HANDLER_KIND_DMA, 3, sdfIpuFromDmacHandler, -1, 0);
    EnableDmac(3);
    D_003BDAA0 = sdfAddHandler(SDF_HANDLER_KIND_DMA, 4, sdfIpuToDmacHandler, -1, 0);
    EnableDmac(4);
    thread = CreateThreadSimple(sdfIpuDecodeThread, sdfIpuDecodeThStack, 0x800, 0x46);
    D_003BDAB4 = thread;
    _StartThread(thread, 0);
}

s32 sdfIpuClose(void) {
    if (D_003BDA9C != 0) {
        sdfIpuRemoveHandler(D_003BDA9C);
        D_003BDA9C = 0;
    }
    DisableDmac(3);
    if (D_003BDAA0 != 0) {
        sdfIpuRemoveHandler(D_003BDAA0);
        D_003BDAA0 = 0;
    }
    return DisableDmac(4);
}

extern int puts(const char *);

/* Print a string followed by a newline through libc puts. */
void sdfPrintTextLine(const char *text) {
    puts(text);
}

INCLUDE_ASM(const s32, "game/code_002E9708", movieWaitSema);

/* Store the stream-frame draw-path mode; movie viewport setup selects 1. */
void sdfSetStreamFrameDrawMode(u32 value) {
    D_003BD61C = value;
}

void sdfSetGridScaledDrawBounds(s32 column, s32 row, s32 width, s32 height, u32 mode) {
    sdfMovieDrawLeft = column * 0x10 + 0x7000;
    sdfMovieDrawTop = row * 8 + 0x7900;
    sdfMovieDrawRight = sdfMovieDrawLeft + width * 0x10;
    sdfMovieDrawBottom = sdfMovieDrawTop + height * 8;
    sdfMovieDrawColor = mode;
}

INCLUDE_ASM(const s32, "game/code_002E9708", sdfMovieQueueFrameSprite);

extern movieStream_t *movieSemaId;
extern void *movieWaitSema(s32);
extern void sdfMovieQueueFrameSprite(movieStream_t *, SdfPoolNode *);

#include "sdf_movie_draw_packet.h"

void sdfMovieDrawFrame(movieStream_t *movie, SdfPoolNode *pool) {
    SdfMovieDrawPacket *packet;
    sdfDrawTag_t *list;
    u16 width;
    u16 height;
    s32 widthLog2;
    s32 heightLog2;
    u32 bufferWidth;
    u32 basePointer;

    if (movie->soundNode.playbackPhase < 2) {
        return;
    }
    if (D_003BD61C == 1) {
        sdfMovieQueueFrameSprite(movie, pool);
        return;
    }
    if (movie->pacEnabled != 0 && movie->pad21 == 0 &&
        movie->stream.pac->ringLength >= 0x1000) {
        movie->pad21 = 1;
        movieSemaId = movie;
        p2sdSetPcmCallback((u32)movieWaitSema);
        p2sdPlayPcm(movie->packetLimit);
    }

    packet = (SdfMovieDrawPacket *)sdfGrAllocPacketBuff(sizeof(SdfMovieDrawPacket));
    packet->header.dmaTag = 0x100000A;
    height = movie->soundNode.height;
    width = movie->soundNode.width;
    packet->header.vifCommands = 0x5000000A00000000ULL;
    packet->header.gifTag = 0x108B400000008009ULL;
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
    packet->sampling.value = 0x21;
    packet->sampling.registerId = SDF_GS_TEX1_1;
    heightLog2 = height - 1;
    widthLog2 = width - 1;
    bufferWidth = width >> 6;
    EE_MMI_BIT_LENGTH(heightLog2);
    EE_MMI_BIT_LENGTH(widthLog2);
    basePointer = (u32)movie->soundNode.resourceWord >> 6;
    if (bufferWidth == 0) {
        bufferWidth = 1;
    }
    packet->texture.value = ((u64)1 << 34) | ((u64)heightLog2 << 30) |
        (widthLog2 << 26) | (movie->soundNode.audioMode != 0 ? 0x200000 : 0) |
        ((u64)bufferWidth << 14) | (u64)basePointer;
    packet->texture.registerId = SDF_GS_TEX0_1;
    packet->clamp.value = 5;
    packet->clamp.registerId = SDF_GS_CLAMP_1;
    packet->textureAlpha.value = 0x4000000080ULL;
    packet->textureAlpha.registerId = SDF_GS_TEXA;
    packet->color.value = (s32)0x80808080;
    packet->color.registerId = SDF_GS_RGBAQ;
    packet->sourceOrigin.value = 0;
    packet->sourceOrigin.registerId = 3;
    packet->vertexOrigin.value = ((u64)movie->drawZ << 32) |
        (u32)(movie->drawY << 16) | (u32)movie->drawX;
    packet->vertexOrigin.registerId = SDF_GS_XYZ2;
    packet->sourceExtent.value = (height << 20) | (width << 4);
    packet->sourceExtent.registerId = 3;
    packet->vertexExtent.value = ((u64)movie->drawZ << 32) |
        (u32)((movie->drawY + movie->drawHeight) << 16) |
        (u32)(movie->drawX + movie->drawWidth);
    packet->vertexExtent.registerId = SDF_GS_XYZ2;
    list = sdfPrimAllocDrawTag(0);
    sdfDrawTagLink(list, (u32)packet);
    pool->append(pool, list);
}

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD494);

INCLUDE_SDATA(const s32, "game/code_002E9708", p2sdPcmState);

INCLUDE_SDATA(const s32, "game/code_002E9708", p2sdPcmBusy);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4A0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4A4);

INCLUDE_SDATA(const s32, "game/code_002E9708", p2sdSmgBuf);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4B0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4B8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4C0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4C8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4D0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4D8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4E0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4E8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4F0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4F8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD500);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD508);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD510);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD518);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD520);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD528);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD530);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD538);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD540);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD548);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD550);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD558);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD560);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD568);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD570);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD578);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD580);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD588);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD590);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD598);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5A0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5A8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5B0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5B8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5C0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5C8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5D0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5D8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5E0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5E8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5F0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5F8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD600);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD608);

INCLUDE_SDATA(const s32, "game/code_002E9708", sdfGsMemoryTypeFormat);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD614);

INCLUDE_SDATA(const s32, "game/code_002E9708", movieStreamSema);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD61C);

INCLUDE_SDATA(const s32, "game/code_002E9708", sdfMovieDrawLeft);

INCLUDE_SDATA(const s32, "game/code_002E9708", sdfMovieDrawTop);

INCLUDE_SDATA(const s32, "game/code_002E9708", sdfMovieDrawRight);

INCLUDE_SDATA(const s32, "game/code_002E9708", sdfMovieDrawBottom);

INCLUDE_SDATA(const s32, "game/code_002E9708", sdfMovieDrawColor);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD634);
