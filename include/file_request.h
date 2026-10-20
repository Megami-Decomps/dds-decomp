#ifndef FILE_REQUEST_H
#define FILE_REQUEST_H

#include "common.h"
#include "sdf_pac_state.h"

struct SdfMemBlock;

/* The fields of a file manager request, whatever its kind. Every kind starts
 * with the same header (0x00-0x2F). It waits in the manager's queue through
 * next, then in its completion list through completionNext.
 *
 * - PAC requests (kind 1) embed their dispatch packet at 0x30 and queue their
 *   completion callbacks through callbackNext.
 * - Device-transfer jobs use the same bytes for state not yet identified.
 * - Callback (kind 0) and value-pair (kind 2) requests are allocated with the
 *   header alone (FILE_REQUEST_HEADER_BYTES).
 *
 * The manager, its core and PAC clients each see a request through their own
 * struct type (FileRequest, FileTransferJob, FilePacRequest), as retail's
 * code distinguishes them. The types share these fields, and so their layout.
 * Link is the type's own name; Callback is how it types the completion
 * callback. */
typedef void (*FileRequestCallback)(void *request, u32 arg);

#define FILE_REQUEST_FIELDS(Link, Callback)                                  \
    u8 kind;                              /* 0x00 */                         \
    u8 state;                             /* 0x01 */                         \
    u8 retryCount;                        /* 0x02 */                         \
    u8 allocationMode;                    /* 0x03: a callback request's mode */ \
    Link *next;                           /* 0x04 */                         \
    char *name;                           /* 0x08: the duplicated request name */ \
    void *deviceRequest;                  /* 0x0C */                         \
    s32 transferBytes;                    /* 0x10: a value-pair request's byte count */ \
    s32 totalBytes;                       /* 0x14 */                         \
    Callback completionCallback;          /* 0x18 */                         \
    void *completionArg;                  /* 0x1C */                         \
    struct SdfMemBlock *allocationHandle; /* 0x20: descriptor passed to sdfMemLock */ \
    u32 retainedAddress;                  /* 0x24: a value-pair request's source data */ \
    u32 transferAddress;                  /* 0x28 */                         \
    Link *completionNext;                 /* 0x2C */                         \
    PacState packet;                      /* 0x30: PAC requests */           \
    u16 readinessEnabled;                 /* 0x68: a gate on completion, not a state value */ \
    u16 slot;                             /* 0x6A: the device-read slot */   \
    Link *callbackNext;                   /* 0x6C: PAC completion callback queue link */

/* A request as the file manager sees it. */
typedef struct FileRequest {
    FILE_REQUEST_FIELDS(struct FileRequest, void *)
} FileRequest;

#define FILE_REQUEST_HEADER_BYTES ((u32)&((FileRequest *)0)->packet)

typedef char FileRequest_size_must_be_0x70[(sizeof(FileRequest) == 0x70) ? 1 : -1];
typedef char FileRequest_header_must_be_0x30[(FILE_REQUEST_HEADER_BYTES == 0x30) ? 1 : -1];
typedef char FileRequest_deviceRequest_offset_check[
    ((u32)&((FileRequest *)0)->deviceRequest == 0x0C) ? 1 : -1];
typedef char FileRequest_completionCallback_offset_check[
    ((u32)&((FileRequest *)0)->completionCallback == 0x18) ? 1 : -1];
typedef char FileRequest_allocationHandle_offset_check[
    ((u32)&((FileRequest *)0)->allocationHandle == 0x20) ? 1 : -1];
typedef char FileRequest_completionNext_offset_check[
    ((u32)&((FileRequest *)0)->completionNext == 0x2C) ? 1 : -1];
typedef char FileRequest_queue_must_be_at_0x60[
    ((u32)&((FileRequest *)0)->packet.queueHead == 0x60) ? 1 : -1];
typedef char FileRequest_readinessEnabled_offset_check[
    ((u32)&((FileRequest *)0)->readinessEnabled == 0x68) ? 1 : -1];
typedef char FileRequest_slot_offset_check[
    ((u32)&((FileRequest *)0)->slot == 0x6A) ? 1 : -1];
typedef char FileRequest_callbackNext_offset_check[
    ((u32)&((FileRequest *)0)->callbackNext == 0x6C) ? 1 : -1];

#endif /* FILE_REQUEST_H */
