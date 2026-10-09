#ifndef SDF_PACKED_RESOURCE_H
#define SDF_PACKED_RESOURCE_H

#include "common.h"

/* The first 0x20 bytes precede the relocatable payload in this resource format. */
#define SDF_PACKED_RESOURCE_HEADER_BYTES 0x20

typedef struct SdfPackedRelocationHeader {
    u8 unknown00[0x10];
    s32 relocationOffset; /* Relative to the payload at +0x20. */
    u32 relocationByteCount;
    u8 unknown18[8];
} SdfPackedRelocationHeader;

typedef char SdfPackedRelocationHeader_size_must_be_20[
    (sizeof(SdfPackedRelocationHeader) == SDF_PACKED_RESOURCE_HEADER_BYTES) ? 1 : -1];
typedef char SdfPackedRelocationHeader_offset_offset_must_be_0x10[
    ((u32)&((SdfPackedRelocationHeader *)0)->relocationOffset == 0x10) ? 1 : -1];
typedef char SdfPackedRelocationHeader_byteCount_offset_must_be_0x14[
    ((u32)&((SdfPackedRelocationHeader *)0)->relocationByteCount == 0x14) ? 1 : -1];

void *sdfRelocatePackedResourcePayload(SdfPackedRelocationHeader *resource);
void *sdfRelocatePackedResourceWordsFromHeader(SdfPackedRelocationHeader *resource);

struct SdfMemBlock;
struct SdfMemBlock *sdfLoadPackedResourceWithRelocatedPayload(const char *name, s32 *outPayloadAddress);
struct SdfMemBlock *sdfReadPackedResourceAndRelocateHeader(const char *name, s32 *outPayloadAddress);

#endif /* SDF_PACKED_RESOURCE_H */
