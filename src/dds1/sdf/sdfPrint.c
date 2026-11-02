#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
#include "sdf_file_existence_api.h"
#include "sdf_console_packet.h"
#include "sif_rpc_client_prefix.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "sdf_timer.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "sdf_draw.h"
#include "sdf_packet_alloc.h"
#include "sdf_sif_command.h"
#include "sdf_dev_event.h"
#include "sdf_dev_protocol.h"
#include "sdf_dev_state.h"
#include "sdf_dev_path_prefix.h"
#include "sdf_dev_worker.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_dev_start_load_api.h"
#include "sdf_disc_file_record.h"
#include "btl.h"
#include "sdf_debug_api.h"

#define SDF_DEV_RPC_QUEUE_BYTES 0x20

#define SDF_DEV_RPC_SERVER_BYTES 0x50

#define SDF_DEV_RPC_SERVER_ID 0x32647270

#define SDF_DEV_RPC_CLIENT_ID 0x646E7270

#define SDF_DEV_RPC_THREAD_STACK_BYTES 0x1000

#define SDF_DEV_RPC_THREAD_PRIORITY 0x4C

#define SDF_DEV_RPC_BIND_POLL_TICKS 4

extern u8 sdfPrintRpcBuff[];

extern void sdfPuts(const char *text);

/* Copy a positive source/end span into the RPC reply buffer; otherwise return NULL. */
void *sdfPrintRpcFunc(s32 unused, SifCommand *packet) {
    s32 copyBytes = packet->end - packet->source;

    if (copyBytes > 0) {
        memcpy(packet, (void *)packet->source, copyBytes);
        return packet;
    }
    return NULL;
}

/* Register the EE-side buffer service and run its RPC queue on the current thread. */
void sdfPrintRpcThread(void) {
    u8 rpcQueue[SDF_DEV_RPC_QUEUE_BYTES];
    u8 rpcServer[SDF_DEV_RPC_SERVER_BYTES];
    sceSifSetRpcQueue(rpcQueue, GetThreadId());
    sceSifRegisterRpc(rpcServer, SDF_DEV_RPC_SERVER_ID, sdfPrintRpcFunc, sdfPrintRpcBuff, 0, 0, rpcQueue);
    sceSifRpcLoop(rpcQueue);
}

extern SifRpcClientContextView sdfDevRpcClient;

extern s8 D_003BD3C8;

extern char D_003BD3D0[];

extern s32 scePrintf(const char *, ...);

/* Start the RPC server thread and bind to the remote service, polling every 4 timer ticks. */
void sdfStartDevRpcServerAndBindClient(void) {
    s32 waitStart;

    _StartThread(sdfCreateThreadWithStack(sdfPrintRpcThread, SDF_DEV_RPC_THREAD_STACK_BYTES, SDF_DEV_RPC_THREAD_PRIORITY), 0);
    while (sceSifMBindRpc(&sdfDevRpcClient.client, SDF_DEV_RPC_CLIENT_ID, 0) >= 0) {
        if (sdfDevRpcClient.client.server != NULL) {
            D_003BD3C8 = 1;
            break;
        }
        waitStart = sdfGetRunTimeCountBase();
        while ((s32)sdfGetRunTimeCount(waitStart) < SDF_DEV_RPC_BIND_POLL_TICKS) {
        }
    }
}

/* Print text through the bound IOP dev client, or locally when it is unbound. */
void sdfPuts(const char *text) {
    u8 buffer[0x13F];
    u8 *alignedBuffer;
    s32 length;

    if (D_003BD3C8 != 0) {
        length = strlen(text);
        if (length != 0) {
            alignedBuffer = (u8 *)(((u32)&buffer[0x3F]) & ~0x3F);
            memcpy(alignedBuffer, text, length);
            sceSifMCallRpc(&sdfDevRpcClient.client, 0x7D1, 0, alignedBuffer, length, NULL, 0, NULL, NULL);
        }
    } else {
        scePrintf(D_003BD3D0, text);
    }
}

s32 sdfPrintf(const char *fmt, ...) {
    char buffer[0x100];
    __builtin_va_list args;
    s32 length;

    __builtin_stdarg_start(args, fmt);
    length = vsprintf(buffer, fmt, args);
    sdfPuts(buffer);
    return length;
}
INCLUDE_SDATA(const s32, "sdf/sdfPrint", D_003BD3C8);

INCLUDE_SDATA(const s32, "sdf/sdfPrint", D_003BD3D0);

