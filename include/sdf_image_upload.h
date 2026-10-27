#ifndef SDF_IMAGE_UPLOAD_H
#define SDF_IMAGE_UPLOAD_H

#include "common.h"

struct sdfMemHandle_t;

/* The deferred worker releases pixels only for the selected owned heap. */
enum {
    SDF_UPLOAD_BORROWED = 0,
    SDF_UPLOAD_GENERAL_HEAP = 1,
    SDF_UPLOAD_CHIP_HEAP = 2
};

/* Pixel upload inputs shared by texture and font-atlas producers. */
typedef struct SdfImageUploadRequest {
    u8 *pixels;
    struct sdfMemHandle_t *allocation;
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

/* Copy descriptor metadata into the queue before returning; the request itself
 * is not retained. Pixel storage may outlive it until DMA completion, with
 * allocationMode selecting whether the queue releases that storage. */
void sdfImageSetReq(const SdfImageUploadRequest *request);

#endif
