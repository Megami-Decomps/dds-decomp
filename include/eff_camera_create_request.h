#ifndef EFF_CAMERA_CREATE_REQUEST_H
#define EFF_CAMERA_CREATE_REQUEST_H

#include "common.h"

struct EffFileJobRequest;
struct EffQueuedFileObject;

typedef struct EffCameraCreateRequest {
    struct EffFileJobRequest *resource;
    struct EffQueuedFileObject *object;
    u32 existingJob;
    f32 position[3];
} EffCameraCreateRequest;

typedef char EffCameraCreateRequest_size_must_be_0x18[
    (sizeof(EffCameraCreateRequest) == 0x18) ? 1 : -1];

#endif
