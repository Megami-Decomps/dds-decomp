#ifndef SDK_LIBMRPC_H
#define SDK_LIBMRPC_H

#include "common.h"

typedef struct sceSifMClientData sceSifMClientData;
typedef void (*sceSifMEndFunc)(void *argument);

s32 sceSifMCallRpc(sceSifMClientData *client, u32 function, u32 mode,
                   void *send, s32 sendSize, void *receive, s32 receiveSize,
                   sceSifMEndFunc end, void *endArgument);

#endif
