#include "sdk_ps2.h"
#include "common.h"
#include "sdf_timer.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "sdf.h"
#include "sdf_pending.h"
#include "sdf_named_resource_request_api.h"
#include "sif_rpc_client_prefix.h"
#include "sdf_mem_block_state.h"

#define SDF_NAMED_RESOURCE_RPC_ID 0x6F496453

#define SDF_RPC_BIND_RETRY_TICKS 0x1ED2

extern SifRpcClientDataPrefix sdfIopRpcClient;

/* Load the optional module first, then the required one; bind retries wait in timer ticks. */
void SdfIopInit(const char *modulePath, const char *optionalModulePath) {
    s32 retryStartTick;

    if (optionalModulePath != NULL) {
        while (sceSifLoadModule(optionalModulePath, 0, NULL) < 0) {
        }
    }
    while (sceSifLoadModule(modulePath, 0, NULL) < 0) {
    }
    if (optionalModulePath != NULL) {
        sceSifMInitRpc(0);
    }
    while (1) {
        sceSifMBindRpc(&sdfIopRpcClient, SDF_NAMED_RESOURCE_RPC_ID, 0);
        if (sdfIopRpcClient.server != NULL) {
            break;
        }
        retryStartTick = sdfGetRunTimeCountBase();
        while ((s32)sdfGetRunTimeCount(retryStartTick) < SDF_RPC_BIND_RETRY_TICKS) {
        }
    }
}
