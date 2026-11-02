#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_timer.h"
#include "sdf_chip.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "ee_mmi.h"
#include "sdf_texture_file.h"
#include "sdf_named_resource_request_api.h"
#include "sce_io.h"
#include "sdf_dev_state.h"
#include "sdf_dev_event.h"
#include "sdf_pac_state.h"
#include "sdf_pac_decoder.h"
#include "sdf_pac_work.h"
#include "sdf_key_pool.h"
#include "sdf_packet_alloc.h"
#include "sdf_stream_state.h"

enum {
    SDF_TMX_MAGIC = 0x30584D54,
    SDF_STREAM_CHUNK_BYTES = 0x4000,
    SDF_STREAM_COMMAND_CREATE_STATE = 32,
    SDF_STREAM_COMMAND_CONTROL = 33,
    SDF_STREAM_COMMAND_READ = 34,
    SDF_STREAM_COMMAND_RELEASE_STATE = 35,
    SDF_IOP_MODULES_PER_GROUP = 2,
    SDF_KEY_TREE_PATH_LIMIT = 32,
    SDF_POOL_REBUILD_CHAIN = 0,
    SDF_POOL_PUSH_FREE = 1
};

/* Initialize a zeroed TMX0 header with the caller's palette metadata. */
void sdfTmxSetHeader(SdfTextureFileHeader *header, s32 width, s32 height, s32 depth, s32 paletteCount, s32 clutFormat) {
    memset(header, 0, sizeof(SdfTextureFileHeader));
    header->clutFormat = clutFormat;
    header->width = width;
    header->height = height;
    header->pixelFormat = depth;
    header->paletteCount = paletteCount;
    header->magic = SDF_TMX_MAGIC;
    header->unk00 = 2;
}

extern SdfStreamState D_003FF340;

extern u8 D_003FF4C0[];

extern s32 D_003FF480[];

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path,
                                          s32 (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32),
                                          void *callbackContext);

extern SdfFileCommandQueueResult sdfFileCmdRequestFileSize(sdfFileCmdHandle_t *state);

extern s32 sdfFileRpcReadCallback(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32);

/* Queue at most one staging-buffer-sized read; the completion callback advances the byte counts. */
void sdfFileRpcContinueRead(void) {
    s32 chunkBytes = D_003FF340.remainingBytes;

    if (chunkBytes > SDF_STREAM_CHUNK_BYTES) {
        chunkBytes = SDF_STREAM_CHUNK_BYTES;
    }
    sdfFileCmdRead(D_003FF340.deviceState, D_003FF4C0, chunkBytes);
}

/* Handle stream replies and DMA-copy read chunks, requesting the next chunk as needed.
 * Uses the shared stream, ignores deviceState/callbackArg, and always returns zero. */
s32 sdfFileRpcReadCallback(sdfFileCmdHandle_t *deviceState, SdfDevEvent command, s32 sourceAddress,
                  s32 byteCount, s32 callbackArg) {
    SdfStreamState *stream = &D_003FF340;
    s32 dmaId;
    s32 timer;

    switch (command) {
    case SDF_DEV_EVENT_SIZE_REPLY:
        stream->readResult = byteCount;
        SignalSema(stream->semaphore);
        break;
    case SDF_DEV_EVENT_READ_REPLY: {
        struct {
            s32 source;
            s32 destination;
            s32 size;
            s32 attributes;
        } transfer;

        FlushCache(0);
        transfer.source = sourceAddress;
        transfer.destination = (s32)stream->destination;
        transfer.size = byteCount;
        transfer.attributes = 0;
        dmaId = sceSifSetDma(&transfer, 1);
        while (sceSifDmaStat(dmaId) >= 0) {
            timer = sdfGetRunTimeCountBase();
            while ((s32)sdfGetRunTimeCount(timer) <= 0) {
            }
        }
        timer = sdfGetRunTimeCountBase();
        while ((s32)sdfGetRunTimeCount(timer) < 0x64) {
        }
        stream->destination += byteCount;
        stream->transferredBytes += byteCount;
        stream->remainingBytes -= byteCount;
        if (stream->remainingBytes > 0) {
            sdfFileRpcContinueRead();
        } else {
            SignalSema(stream->semaphore);
        }
        break;
    }
    case SDF_DEV_EVENT_OPENED:
    case SDF_DEV_EVENT_CLOSED:
        SignalSema(stream->semaphore);
        break;
    case SDF_DEV_EVENT_INACTIVE:
        SignalSema(stream->semaphore);
        break;
    }
    return 0;
}

/* Dispatch a stream command, wait for completion, and return the shared reply buffer.
 * request is a scalar argument except for a read command, which carries a read-request address.
 * An unrecognized command leaves the shared reply unchanged. */
s32 *sdfFileRpcHandler(u32 command, s32 request) {
    SdfStreamState *state = &D_003FF340;
    sdfFileCmdHandle_t *deviceState;
    switch (command) {
    case SDF_STREAM_COMMAND_CREATE_STATE:
        state->deviceState = sdfFileCmdReadOpen((const char *)request, sdfFileRpcReadCallback, 0);
        if (state->deviceState != 0) {
            WaitSema(state->semaphore);
            deviceState = state->deviceState;
            if (deviceState->result != 0) {
                sdfFileCmdClearError(deviceState);
                sdfFileCmdFree(state->deviceState);
                state->deviceState = 0;
                deviceState = 0;
            }
        } else {
            deviceState = 0;
        }
        D_003FF480[0] = (s32)deviceState;
        break;
    case SDF_STREAM_COMMAND_CONTROL:
        sdfFileCmdRequestFileSize(state->deviceState);
        WaitSema(state->semaphore);
        D_003FF480[0] = state->readResult;
        break;
    case SDF_STREAM_COMMAND_READ:
        state->remainingBytes = ((SdfStreamReadRequest *)request)->byteCount;
        state->transferredBytes = 0;
        state->destination = (u8 *)((SdfStreamReadRequest *)request)->destinationAddress;
        sdfFileRpcContinueRead();
        WaitSema(state->semaphore);
        D_003FF480[0] = state->transferredBytes;
        break;
    case SDF_STREAM_COMMAND_RELEASE_STATE:
        sdfFileCmdClose(state->deviceState);
        WaitSema(state->semaphore);
        sdfFileCmdFree(state->deviceState);
        state->deviceState = 0;
        D_003FF480[0] = 0;
        break;
    }
    return D_003FF480;
}

extern s32 D_003BDAC4;

extern u8 D_003FF380[];

extern u8 sdfHddValid;

/* Load SDFIOPFL, wake the creator, and serve the registered stream RPC commands. */
void SdfFileRpcServerThreadProc(void) {
    SdfStreamState *stream = &D_003FF340;
    SceIoStat info;
    u8 queue[0x20];
    u8 server[0x50];
    char *module;

    stream->semaphore = sdfCreateSema(0, 1, 0);
    sceSifSetRpcQueue(queue, GetThreadId());
    sceSifRegisterRpc(server, 0x66504F49, sdfFileRpcHandler,
                      D_003FF380, 0, 0, queue);
    module = "cdrom0:\\USERIRX\\SDFIOPFL.IRX;1";
    if (sdfHddValid != 0 &&
        sceGetstat("pfs0:/userirx/sdfiopfl.irx", &info) == 0 &&
        (info.mode & 0xF000) == 0x2000) {
        module = "pfs0:/userirx/sdfiopfl.irx";
    }
    SdfIopFileRpc(module, 0, NULL, NULL);
    WakeupThread(D_003BDAC4);
    sceSifRpcLoop(queue);
}

/* Start the newly created worker, then record and suspend the calling thread. */
void sdfStartFileRpcServer(void) {
    s32 workerId = sdfCreateThreadWithStack(SdfFileRpcServerThreadProc, 0x1000, 0x4C);

    _StartThread(workerId, 0);
    D_003BDAC4 = GetThreadId();
    SleepThread();
}

extern char *D_00398C80[];

extern void sceMcInit(void);

/* Load the selected pair of IOP module paths from prefix; loader results are not checked. */
void LoadMcModules(const char *prefix, s32 moduleGroup) {
    char path[0x100];
    char **moduleNames = &D_00398C80[moduleGroup * SDF_IOP_MODULES_PER_GROUP];
    s32 moduleIndex;

    for (moduleIndex = 0; moduleIndex != SDF_IOP_MODULES_PER_GROUP; moduleIndex++) {
        strcpy(path, prefix);
        strcat(path, moduleNames[moduleIndex]);
        sceSifLoadModule(path, 0, 0);
    }
    sceMcInit();
}

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_003B50C0);

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_003B50D0);

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_003B50E0);

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_003B50F0);

