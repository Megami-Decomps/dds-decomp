#ifndef SMT3_SIF_RPC_H
#define SMT3_SIF_RPC_H

#include "common.h"

/* EE SIF RPC queue and server records shared by file and sound services. */

typedef struct sifRpcServerData_t sifRpcServerData_t;

typedef struct sifRpcDataQueue_t {
    s32 threadId;
    s32 active;
    sifRpcServerData_t *link;
    sifRpcServerData_t *start;
    sifRpcServerData_t *end;
    struct sifRpcDataQueue_t *next;
} sifRpcDataQueue_t;

typedef void *(*sifRpcFunc_t)(s32 function, void *request, s32 requestSize);

struct sifRpcServerData_t {
    s32 serviceId;
    sifRpcFunc_t handler;
    void *buffer;
    s32 size;
    sifRpcFunc_t callback;
    void *callbackBuffer;
    s32 size2;
    void *client;
    void *packetAddress;
    s32 rpcNumber;
    void *receiveBuffer;
    s32 receiveSize;
    s32 mode;
    s32 requestId;
    sifRpcServerData_t *link;
    sifRpcServerData_t *next;
    sifRpcDataQueue_t *queue;
};

typedef char sifRpcDataQueue_size_must_be_0x18[
    sizeof(sifRpcDataQueue_t) == 0x18 ? 1 : -1];
typedef char sifRpcServerData_size_must_be_0x44[
    sizeof(sifRpcServerData_t) == 0x44 ? 1 : -1];

extern void sceSifSetRpcQueue(sifRpcDataQueue_t *queue, s32 threadId);
#include "sdk_libkernl.h"

extern void sceSifRpcLoop(sifRpcDataQueue_t *queue);

#endif
