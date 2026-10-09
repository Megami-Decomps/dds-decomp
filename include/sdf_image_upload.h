#ifndef SDF_IMAGE_UPLOAD_H
#define SDF_IMAGE_UPLOAD_H

#include "common.h"

struct SdfMemBlock;

/* Pixel upload inputs shared by texture and font-atlas producers. */
typedef struct SdfImageUploadRequest {
    void *pixels;
    struct SdfMemBlock *allocation;
    u8 allocationMode;
    u8 format;
    u16 bufferWidth;
    u32 destination;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} SdfImageUploadRequest;

typedef char SdfImageUploadRequest_layout[
    (sizeof(SdfImageUploadRequest) == 0x18 &&
     (u32)&((SdfImageUploadRequest *)0)->allocation == 0x04 &&
     (u32)&((SdfImageUploadRequest *)0)->allocationMode == 0x08 &&
     (u32)&((SdfImageUploadRequest *)0)->destination == 0x0C &&
     (u32)&((SdfImageUploadRequest *)0)->x == 0x10) ? 1 : -1];

#endif
