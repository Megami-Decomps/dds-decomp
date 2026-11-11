#ifndef EFF_CAMERA_CREATE_REQUEST_H
#define EFF_CAMERA_CREATE_REQUEST_H

#include "common.h"
#include "eff_file_job_request.h"

struct EffQueuedFileObject;

typedef struct EffCameraCreateRequest {
    struct EffFileJobRequest *resource;
    struct EffQueuedFileObject *object;
    const char *existingJobName;
    f32 position[3];
} EffCameraCreateRequest;

typedef char EffCameraCreateRequest_size_must_be_0x18[
    (sizeof(EffCameraCreateRequest) == 0x18) ? 1 : -1];

typedef struct EffCameraCreateRow {
    EffCameraCreateRequest request;
    u32 sourceTail;
} EffCameraCreateRow;

typedef char EffCameraCreateRowSizeCheck[(sizeof(EffCameraCreateRow) == 0x1C) ? 1 : -1];

#endif
