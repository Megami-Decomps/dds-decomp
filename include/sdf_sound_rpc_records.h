#ifndef SDF_SOUND_RPC_RECORDS_H
#define SDF_SOUND_RPC_RECORDS_H

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
