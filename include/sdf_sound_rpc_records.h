#ifndef SDF_SOUND_RPC_RECORDS_H
#define SDF_SOUND_RPC_RECORDS_H

#include "common.h"

enum SdfSoundRpcCommand {
    SDF_SOUND_RPC_OPEN_VOICE_FILE = 0,
    SDF_SOUND_RPC_READ_VOICE_FILE = 1,
    SDF_SOUND_RPC_CLOSE_VOICE_FILE = 2,
    SDF_SOUND_RPC_RESET_RESIDENT_BUFFER_OFFSET = 3,
    SDF_SOUND_RPC_READ_RESIDENT_BUFFER = 4,
    SDF_SOUND_RPC_GET_PCM_CALLBACK_WORD = 6,
    SDF_SOUND_RPC_READ_PCM_CALLBACK_BUFFER = 7,
    SDF_SOUND_RPC_CLEAR_PCM_CALLBACK_BUSY = 8
};

/* The PCM RPC borrows this read-only span for its IOP copy; NULL means no data. */
typedef const void *(*SdfSoundReadCallback)(s32 byteCount);

typedef union SdfSoundCommand {
    u32 word; /* Callback address returned to the IOP. */
    SdfSoundReadCallback read;
} SdfSoundCommand;

typedef char SdfSoundCommand_size_must_be_4[
    (sizeof(SdfSoundCommand) == 4) ? 1 : -1];

/* While idle, stores the callback (NULL clears it) and returns it; while busy,
 * leaves the current callback installed and returns NULL. */
SdfSoundReadCallback p2sdSetPcmCallback(SdfSoundReadCallback callback);

typedef struct SdfSoundRpcRequest {
    u8 opaque00[8];
    s32 destination; /* 0x08: destination address for the IOP copy */
    s32 byteCount;   /* 0x0C: transfer size */
} SdfSoundRpcRequest;

typedef struct SdfSoundResidentBuffer {
    u8 *data; /* 0x00: resident buffer base */
    u8 opaque04[0x24];
} SdfSoundResidentBuffer;

typedef char SdfSoundRpcRequestLayoutAssert[
    (sizeof(SdfSoundRpcRequest) == 0x10 &&
     (u32)&((SdfSoundRpcRequest *)0)->destination == 8 &&
     (u32)&((SdfSoundRpcRequest *)0)->byteCount == 0xC) ? 1 : -1];

typedef char SdfSoundResidentBufferDataOffsetAssert[
    ((u32)&((SdfSoundResidentBuffer *)0)->data == 0) ? 1 : -1];

#endif
