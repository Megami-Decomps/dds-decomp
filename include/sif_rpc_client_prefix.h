#ifndef SIF_RPC_CLIENT_PREFIX_H
#define SIF_RPC_CLIENT_PREFIX_H

#include "common.h"

/* Named-resource RPC users observe the SDK client only through its server slot. */
typedef struct SifRpcClientDataPrefix {
    u8 opaque00[0x24];
    void *server; /* 0x24: set by a successful bind */
} SifRpcClientDataPrefix;

typedef char SifRpcClientDataPrefix_server_at_0x24[
    ((u32)&((SifRpcClientDataPrefix *)0)->server == 0x24) ? 1 : -1];

typedef char SifRpcClientDataPrefix_size_must_be_0x28[
    (sizeof(SifRpcClientDataPrefix) == 0x28) ? 1 : -1];

/* These dev-RPC globals append eight opaque bytes after the SDK client prefix. */
typedef struct SifRpcClientContextView {
    SifRpcClientDataPrefix client;
    u8 opaque28[8];
} SifRpcClientContextView;

typedef char SifRpcClientContextView_size_must_be_0x30[
    (sizeof(SifRpcClientContextView) == 0x30) ? 1 : -1];
typedef char SifRpcClientContextView_server_at_0x24[
    ((u32)&((SifRpcClientContextView *)0)->client.server == 0x24) ? 1 : -1];

/* The multi-RPC library (sdk/libmrpc) on these clients: initialize, bind to
 * an IOP server, and call it. Calls take 64-byte-aligned buffers and an
 * optional end function. */
typedef void (*SifMRpcEnd)(void *argument);

s32 sceSifMBindRpc(SifRpcClientDataPrefix *client, s32 id, s32 mode);
s32 sceSifMCallRpc(SifRpcClientDataPrefix *client, s32 function, s32 mode, void *send, s32 sendSize,
                   void *receive, s32 receiveSize, SifMRpcEnd end, void *endArgument);
void sceSifMInitRpc(s32 mode);

#endif
