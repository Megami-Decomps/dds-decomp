#ifndef EFF_FILE_JOB_REQUEST_H
#define EFF_FILE_JOB_REQUEST_H

#include "common.h"

struct SdfMemBlock;

typedef struct EffFileJobRequest {
    char *name;                       /* 0x00 */
    u16 fileKind;                     /* 0x04 */
    u8 pad06[2];
    u16 transferMode;                /* 0x08 */
    u8 pad0A[2];
    void *output;                     /* 0x0C */
    u32 size;                         /* 0x10 */
    struct SdfMemBlock *allocation;   /* 0x14 */
    u16 resourceMode;                 /* 0x18 */
    u8 pad1A[2];
    const char *secondaryCommandPath; /* 0x1C */
} EffFileJobRequest;

typedef char EffFileJobRequestSizeCheck[(sizeof(EffFileJobRequest) == 0x20) ? 1 : -1];
typedef char EffFileJobRequestTransferModeOffsetCheck[
    ((u32)&((EffFileJobRequest *)0)->transferMode == 0x08) ? 1 : -1];
typedef char EffFileJobRequestOutputOffsetCheck[
    ((u32)&((EffFileJobRequest *)0)->output == 0x0C) ? 1 : -1];
typedef char EffFileJobRequestAllocationOffsetCheck[
    ((u32)&((EffFileJobRequest *)0)->allocation == 0x14) ? 1 : -1];
typedef char EffFileJobRequestSecondaryPathOffsetCheck[
    ((u32)&((EffFileJobRequest *)0)->secondaryCommandPath == 0x1C) ? 1 : -1];

#endif
