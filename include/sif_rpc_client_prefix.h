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

#endif
