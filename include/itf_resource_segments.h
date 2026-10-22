#ifndef ITF_RESOURCE_SEGMENTS_H
#define ITF_RESOURCE_SEGMENTS_H

#include "common.h"
#include "itf_bit_range.h"

/* Relative offsets select three packed-font resource regions. */
typedef struct ItfRelativeSegmentResourceView {
    s32 firstOffset; /* 0x00 */
    s32 secondDelta; /* 0x04 */
    u8 opaque08[0x10];
    s32 thirdDelta; /* 0x18 */
} ItfRelativeSegmentResourceView;

/* Output pointers address the tree, entry bit ranges, and packed bit words. */
typedef struct ItfPackedResourceSegments {
    u8 *treeNodes; /* 0x00: six-byte prefix-tree nodes. */
    ItfBitRange *entryBitRanges; /* 0x04: one start/end bit span per entry. */
    u16 *packedBitstream; /* 0x08: packed bit words. */
} ItfPackedResourceSegments;

typedef char ItfRelativeFirstOffsetCheck[((u32)&((ItfRelativeSegmentResourceView *)0)->firstOffset == 0x00) ? 1 : -1];
typedef char ItfRelativeSecondDeltaOffsetCheck[((u32)&((ItfRelativeSegmentResourceView *)0)->secondDelta == 0x04) ? 1 : -1];
typedef char ItfRelativeThirdDeltaOffsetCheck[((u32)&((ItfRelativeSegmentResourceView *)0)->thirdDelta == 0x18) ? 1 : -1];
typedef char ItfPackedSegmentsSizeCheck[(sizeof(ItfPackedResourceSegments) == 0x0C) ? 1 : -1];
typedef char ItfPackedOffsetsOffsetCheck[((u32)&((ItfPackedResourceSegments *)0)->entryBitRanges == 0x04) ? 1 : -1];
typedef char ItfPackedBitstreamOffsetCheck[((u32)&((ItfPackedResourceSegments *)0)->packedBitstream == 0x08) ? 1 : -1];

void frGetFontImagePtrs(ItfRelativeSegmentResourceView *resource,
                              ItfPackedResourceSegments *segments);
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex,
                                void *resourceBase,
                                ItfPackedResourceSegments *segments);

#endif
