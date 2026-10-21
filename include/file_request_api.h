#ifndef FILE_REQUEST_API_H
#define FILE_REQUEST_API_H

#include "common.h"
#include "file_request_callback.h"

struct SdfMemBlock;

/* Return the request's allocation descriptor without changing ownership. */
struct SdfMemBlock *fileGetResourceHandle(const struct FileRequest *request);
/* Return the address of the loaded resource payload as a byte pointer. */
u8 *fileGetLoadedDataAddress(const struct FileRequest *request);
u32 fileGetResourceSize(const struct FileRequest *request);
s32 filePollEntryCleanup(struct FileRequest *request);

#ifdef VERSION_DDS1
struct FileRequest *fileAllocateDispatchRequest(const char *requestName, u32 flags, u32 dispatchValue,
                                                FileRequestCallback onComplete, void *userData);
struct FileRequest *fileCreateCallbackRequest(const char *requestName, u32 callbackMode,
                                              FileRequestCallback callback, void *userData);
#else
struct FileRequest *fileCreatePacLoadWork(const char *requestName, s32 flags, void *dispatchValue,
                                             FileRequestCallback onComplete, void *userData);
struct FileRequest *fileCreateCallbackRequest(const char *requestName, s32 callbackMode,
                                              FileRequestCallback callback, void *userData);
#endif

struct FileRequest *fileQueuePlainDispatchRequest(const char *requestName);
void fileQueueFlaggedDispatchRequest(const char *requestName);
struct FileRequest *fileQueueDefaultCallbackRequest(const char *requestName);
struct FileRequest *fileQueueAlternateCallbackRequest(const char *requestName);

s32 fileIsRequestReadyInCurrentMode(const struct FileRequest *request);
s32 fileRequestIsReady(const struct FileRequest *request);
void fileWaitReady(struct FileRequest *request);
void fileWaitForRequestReady(struct FileRequest *request);
void fileReqStartCardDetection(u32 port);

struct FileRequest *fileQueueWindowSlotRequest(const char *requestName, const void *sourceData, s32 sourceByteCount);

#endif /* FILE_REQUEST_API_H */
