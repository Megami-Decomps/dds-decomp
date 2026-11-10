#ifndef DDS3_FRAME_DRAW_BUFFER_H
#define DDS3_FRAME_DRAW_BUFFER_H

#include "common.h"

#define DDS3_FRAME_DRAW_BASE_BYTES 0x40
#define DDS3_FRAME_DRAW_BUFFER_STRIDE 0x1F40

/* Shared frame draw-buffer stride. Packet contents beyond the base view stay opaque. */
typedef struct dds3FrameDrawBuffer_t {
    u8 baseBytes[DDS3_FRAME_DRAW_BASE_BYTES];
    u8 unknown40[DDS3_FRAME_DRAW_BUFFER_STRIDE - DDS3_FRAME_DRAW_BASE_BYTES];
} dds3FrameDrawBuffer_t;

typedef char dds3FrameDrawBuffer_size_must_be_0x1F40[
    (sizeof(dds3FrameDrawBuffer_t) == DDS3_FRAME_DRAW_BUFFER_STRIDE) ? 1 : -1];
typedef char dds3FrameDrawBuffer_baseBytes_offset_must_be_0[
    ((u32)&((dds3FrameDrawBuffer_t *)0)->baseBytes == 0) ? 1 : -1];
typedef char dds3FrameDrawBuffer_unknown40_offset_must_be_0x40[
    ((u32)&((dds3FrameDrawBuffer_t *)0)->unknown40 == DDS3_FRAME_DRAW_BASE_BYTES) ? 1 : -1];

#endif /* DDS3_FRAME_DRAW_BUFFER_H */
