#ifndef SDF_TEXTURE_OFFSET_LIST_H
#define SDF_TEXTURE_OFFSET_LIST_H

#include "common.h"

/* Fixed prefix of a serialized texture-offset list. Signed relative offsets
 * begin immediately after this prefix; the enclosing resource owns its extent. */
typedef struct SdfTextureOffsetListHeader {
    u8 unknown00[0x10];
    s32 textureCount;
} SdfTextureOffsetListHeader;

typedef char SdfTextureOffsetListHeader_size_must_be_0x14[
    (sizeof(SdfTextureOffsetListHeader) == 0x14) ? 1 : -1];
typedef char SdfTextureOffsetListHeader_count_at_0x10[
    ((u32)&((SdfTextureOffsetListHeader *)0)->textureCount == 0x10) ? 1 : -1];

struct DevRequest;
struct DevRequest *sndBuildResourceHandleListFromOffsets(
    const SdfTextureOffsetListHeader *resource);

#endif /* SDF_TEXTURE_OFFSET_LIST_H */
