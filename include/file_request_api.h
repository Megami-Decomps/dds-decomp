#ifndef FILE_REQUEST_API_H
#define FILE_REQUEST_API_H

#include "common.h"

struct FileRequest;

#ifdef VERSION_DDS1
void *fileAllocateDispatchRequest(const char *requestName, u32 flags, u32 dispatchValue,
                                 u32 onComplete, u32 userData);
void *fileCreateCallbackRequest(const char *requestName, u32 callbackMode, u32 callbackAddress,
                                u32 userData);
#else
void *fileCreatePacLoadWork(const char *requestName, s32 flags, void *dispatchValue,
                            s32 onComplete, s32 userData);
void *fileCreateCallbackRequest(const char *requestName, s32 callbackMode, s32 callbackAddress,
                                s32 userData);
#endif

void *fileQueuePlainDispatchRequest(const char *requestName);
void fileQueueFlaggedDispatchRequest(const char *requestName);
void *fileQueueDefaultCallbackRequest(const char *requestName);
void *fileQueueAlternateCallbackRequest(const char *requestName);

s32 fileIsRequestReadyInCurrentMode(struct FileRequest *request);
s32 fileRequestIsReady(struct FileRequest *request);
void fileWaitReady(struct FileRequest *request);
void func_00288C50(struct FileRequest *request);
void func_002C81D0(struct FileRequest *request);

struct FileRequest *fileQueueWindowSlotRequest(const char *requestName, void *data, s32 size);

#endif /* FILE_REQUEST_API_H */
