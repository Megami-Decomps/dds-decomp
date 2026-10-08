#ifndef SDF_PENDING_H
#define SDF_PENDING_H

#include "sdf.h"

#define SDF_PENDING_NODE_BYTES 0x10
#define SDF_PENDING_BUFFER_BYTES 0x100
#define SDF_PENDING_BUFFER_CAPACITY 0x3F

/* A queued batch links its next chunk before 63 stored entry words. */
typedef struct SdfPendingBuffer {
    struct SdfPendingBuffer *next;
    u32 entry[SDF_PENDING_BUFFER_CAPACITY];
} SdfPendingBuffer;

/* One pending callback owner's node: global-chain link, owner, data chunk,
 * and remaining free entry slots. */
struct SdfPendingNode {
    SdfPendingNode *next;       /* 0x00 */
    SdfPendingRequest *owner;   /* 0x04 */
    SdfPendingBuffer *buffer;   /* 0x08 */
    s32 remaining;              /* 0x0C */
};

typedef char SdfPendingNode_size_must_be_0x10[(sizeof(SdfPendingNode) == 0x10) ? 1 : -1];
typedef char SdfPendingBuffer_size_must_be_0x100[(sizeof(SdfPendingBuffer) == 0x100) ? 1 : -1];

void sdfInitializeSynchronizedRequest(SdfPendingRequest *request, SdfPendingCallback callback);
void sdfPendingQueuePush(SdfPendingRequest *owner, u32 entry);

#endif
