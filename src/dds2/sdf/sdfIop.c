#include "common.h"

/* The SDK multi-RPC client. sceSifMBindRpc leaves the server handle at 0x24,
 * which is how the bind loop below knows the server answered. */
typedef struct
{
    u8 opaque00[0x24];
    void *serve;
} sdfIopRpcClient_t;

#include "sdk_libc.h"

extern s32 sceSifLoadModule(const char *a, s32 b, const char *c);
extern void sceSifMInitRpc(s32 mode);
extern s32 sceSifMBindRpc(sdfIopRpcClient_t *client, s32 number, s32 mode);
extern s32 sdfGetRunTimeCountBase(void);
extern s32 sdfGetRunTimeCount(s32 aBaseTime);

extern sdfIopRpcClient_t sdfIopRpcClient;

void SdfIopInit(const char *driverPath, const char *modulePath)
{
    s32 startTicks;

    if (modulePath != NULL) {
        while (sceSifLoadModule(modulePath, 0, NULL) < 0) {
        }
    }
    while (sceSifLoadModule(driverPath, 0, NULL) < 0) {
    }
    if (modulePath != NULL) {
        sceSifMInitRpc(0);
    }
    while (1) {
        sceSifMBindRpc(&sdfIopRpcClient, 0x6F496453, 0);
        if (sdfIopRpcClient.serve != NULL) {
            break;
        }
        startTicks = sdfGetRunTimeCountBase();
        while (sdfGetRunTimeCount(startTicks) < 0x1ED2) {
        }
    }
}

