#ifndef FILE_REQUEST_API_H
#define FILE_REQUEST_API_H

#include "common.h"

struct FileRequest;

s32 fileIsRequestReadyInCurrentMode(struct FileRequest *request);
s32 fileRequestIsReady(struct FileRequest *request);
void fileWaitReady(struct FileRequest *request);
void func_00288C50(struct FileRequest *request);
void func_002C81D0(struct FileRequest *request);

struct FileRequest *fileQueueWindowSlotRequest(const char *requestName, void *data, s32 size);

#endif /* FILE_REQUEST_API_H */
