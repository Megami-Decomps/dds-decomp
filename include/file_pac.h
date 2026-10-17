#ifndef FILE_PAC_H
#define FILE_PAC_H

#include "common.h"
#include "file_request.h"
#include "sdf_pac_packet.h"
#include "sdf_pac_state.h"
#include "sdf_pac_work.h"

struct PacAlloc;

/* A PAC archive request (kind 1), with its dispatch packet at 0x30. */
typedef struct FilePacRequest {
    FILE_REQUEST_FIELDS(struct FilePacRequest, void *)
} FilePacRequest;

#endif /* FILE_PAC_H */
