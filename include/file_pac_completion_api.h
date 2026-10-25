#ifndef FILE_PAC_COMPLETION_API_H
#define FILE_PAC_COMPLETION_API_H

#include "file_request_api.h"

/* PAC completion requires both the decoded end marker and request state six. */
typedef s32 FilePacRequestCompletionResult;
enum {
    FILE_PAC_REQUEST_COMPLETION_INCOMPLETE = 0,
    FILE_PAC_REQUEST_COMPLETION_COMPLETE = 1,
};

FilePacRequestCompletionResult pacCheckEnd(const struct FileRequest *request);

#endif /* FILE_PAC_COMPLETION_API_H */
