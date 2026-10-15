#ifndef SDF_TEXTURE_OFFSET_LIST_H
#define SDF_TEXTURE_OFFSET_LIST_H

#include "common.h"

/* One signed displacement from the list base to an SdfTextureFileHeader. */
typedef struct SdfTextureOffsetListEntry {
    s32 textureHeaderOffset;
} SdfTextureOffsetListEntry;

typedef char SdfTextureOffsetListEntry_size_must_be_4[
    (sizeof(SdfTextureOffsetListEntry) == 4) ? 1 : -1];

/* Fixed prefix of a serialized texture-offset list. For a valid nonnegative
 * count, that many entries follow immediately after this 0x14-byte prefix.
 * Each signed displacement is relative to the list base; the enclosing
 * resource owns the entries' extent. The first 0x10 bytes remain opaque. */
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
