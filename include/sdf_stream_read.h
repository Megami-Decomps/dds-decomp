#ifndef SDF_STREAM_READ_H
#define SDF_STREAM_READ_H

#include "common.h"

/* Serialized stream-frame prefix copied before frame payload data. */
typedef struct SdfStreamFrameHeader {
    u8 reserved[8];
    u16 width;
    u16 height;
    u32 cycleLength;
} SdfStreamFrameHeader;

typedef char SdfStreamFrameHeader_size_must_be_0x10[
    (sizeof(SdfStreamFrameHeader) == 0x10) ? 1 : -1];
typedef char SdfStreamFrameHeader_width_offset_must_be_8[
    ((u32)&((SdfStreamFrameHeader *)0)->width == 8) ? 1 : -1];
typedef char SdfStreamFrameHeader_height_offset_must_be_0xA[
    ((u32)&((SdfStreamFrameHeader *)0)->height == 0xA) ? 1 : -1];
typedef char SdfStreamFrameHeader_cycleLength_offset_must_be_0xC[
    ((u32)&((SdfStreamFrameHeader *)0)->cycleLength == 0xC) ? 1 : -1];
#define SDF_STREAM_FRAME_HEADER_BYTES 0x10

/* Operations passed through the SdfStreamRead callback contract. */
enum {
    SDF_STREAM_READ_QUERY = 0,
    SDF_STREAM_READ_COPY = 1,
    SDF_STREAM_READ_RESUME = 2
};

#endif /* SDF_STREAM_READ_H */
