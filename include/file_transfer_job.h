#ifndef FILE_TRANSFER_JOB_H
#define FILE_TRANSFER_JOB_H

#include "common.h"

/* Async device-transfer request shared by the two title file managers.
 * The middle bytes are retained as opaque until their ownership is known. */
typedef struct FileTransferJob {
    u8 kind;
    u8 state;
    u8 retryCount;
    u8 allocationMode;
    struct FileTransferJob *next;
    char *name;
    void *deviceRequest;
    s32 transferBytes;
    s32 totalBytes;
    void (*completionCallback)(void *job, u32 arg);
    u32 completionArg;
    s32 allocationHandle;
    u32 retainedAddress;
    u32 transferAddress;
    struct FileTransferJob *completionNext;
    u8 opaque30[0x38];
    u16 stateRequired;
    u16 slot;
} FileTransferJob;

typedef char FileTransferJob_size_check[
    (sizeof(FileTransferJob) == 0x6C) ? 1 : -1];
typedef char FileTransferJob_deviceRequest_offset_check[
    ((u32)&((FileTransferJob *)0)->deviceRequest == 0x0C) ? 1 : -1];
typedef char FileTransferJob_completionCallback_offset_check[
    ((u32)&((FileTransferJob *)0)->completionCallback == 0x18) ? 1 : -1];
typedef char FileTransferJob_completionNext_offset_check[
    ((u32)&((FileTransferJob *)0)->completionNext == 0x2C) ? 1 : -1];
typedef char FileTransferJob_stateRequired_offset_check[
    ((u32)&((FileTransferJob *)0)->stateRequired == 0x68) ? 1 : -1];
typedef char FileTransferJob_slot_offset_check[
    ((u32)&((FileTransferJob *)0)->slot == 0x6A) ? 1 : -1];

#endif /* FILE_TRANSFER_JOB_H */
