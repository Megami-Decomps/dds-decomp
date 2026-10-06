#ifndef FILE_PAC_H
#define FILE_PAC_H

#include "common.h"

struct FileNode;
struct PacBuf;
struct PacAlloc;

/* PAC queue item: allocation handle and decoder cursor precede packet data. */
typedef struct PacWork {
    struct PacWork *next;
    struct PacState *owner;
    s32 resourceHandle;
    u8 *dataCursor;
    u8 packet[1];
} PacWork;

/* 0x38-byte PAC decoder state; resource packets are linked through queueHead.
 * Event 0 supplies a payload-size word; event 1 omits it. Keep the
 * packet callback's native short-arity interface unprototyped. */
typedef struct PacState {
    u8 phase;
    u8 flags;
    u16 packetCounter;
    s32 (*packetCallback)();
    void (*onInput)(struct PacState *);
    void (*onComplete)(struct PacState *);
    u8 *inputCursor;
    s32 inputAvailable;
    s32 consumedBytes;
    u8 *outputCursor;
    s32 pendingBytes;
    struct PacBuf *decoder;
    struct PacBuf *resourceBuffer;
    struct PacAlloc *allocation;
    PacWork *queueHead;
    PacWork *queueTail;
} PacState;

/* 0x70-byte file request: PAC state at 0x30, readiness gate at 0x68. */
typedef struct FilePacRequest {
    u8 kind;
    u8 state;
    u8 pad02[2];
    struct FileNode *next;
    char *name;
    u32 handle;
    s32 size;
    u8 pad14[4];
    void *callback;
    void *userData;
    u8 pad20[0x10];
    PacState packet;
    u16 readinessEnabled;
    u16 slot;
    u8 pad6C[4];
} FilePacRequest;

typedef char PacState_size_must_be_0x38[(sizeof(PacState) == 0x38) ? 1 : -1];
typedef char FilePacRequest_size_must_be_0x70[(sizeof(FilePacRequest) == 0x70) ? 1 : -1];
typedef char FilePacRequest_queue_must_be_at_0x60[
    ((u32)&((FilePacRequest *)0)->packet.queueHead == 0x60) ? 1 : -1];
typedef char PacWork_resource_must_be_at_0x08[
    ((u32)&((PacWork *)0)->resourceHandle == 0x08) ? 1 : -1];

#endif /* FILE_PAC_H */
