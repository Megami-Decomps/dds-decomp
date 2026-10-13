#ifndef FLD_TRANSFER_CHUNK_H
#define FLD_TRANSFER_CHUNK_H

#include "common.h"

/* Packed-resource transfer descriptor; the first word's role is unknown. */
typedef struct FldTransferChunk {
    u32 unk00;     /* 0x00: opaque descriptor word */
    s32 offset;    /* 0x04: signed byte offset from the resource base */
    u32 size;      /* 0x08: byte length of the transferred payload */
} FldTransferChunk;

typedef char FldTransferChunkSizeCheck[(sizeof(FldTransferChunk) == 0x0C) ? 1 : -1];

#endif
