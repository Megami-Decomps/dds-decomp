#ifndef FILE_REQUEST_API_H
#define FILE_REQUEST_API_H

#include "common.h"

struct FileRequest;

/* Loaded resource values remain 32-bit words; only the request owner is opaque. */
u32 fileGetResourceHandle(struct FileRequest *request);
u32 fileGetLoadedDataAddress(struct FileRequest *request);
u32 fileGetResourceSize(struct FileRequest *request);

#ifdef VERSION_DDS1
struct FileRequest *fileAllocateDispatchRequest(const char *requestName, u32 flags, u32 dispatchValue,
                                                u32 onComplete, u32 userData);
struct FileRequest *fileCreateCallbackRequest(const char *requestName, u32 callbackMode,
                                              u32 callbackAddress, u32 userData);
#else
struct FileRequest *fileCreatePacLoadWork(const char *requestName, s32 flags, void *dispatchValue,
                                          s32 onComplete, s32 userData);
struct FileRequest *fileCreateCallbackRequest(const char *requestName, s32 callbackMode,
                                              s32 callbackAddress, s32 userData);
#endif

struct FileRequest *fileQueuePlainDispatchRequest(const char *requestName);
void fileQueueFlaggedDispatchRequest(const char *requestName);
struct FileRequest *fileQueueDefaultCallbackRequest(const char *requestName);
struct FileRequest *fileQueueAlternateCallbackRequest(const char *requestName);

s32 fileIsRequestReadyInCurrentMode(struct FileRequest *request);
s32 fileRequestIsReady(struct FileRequest *request);
void fileWaitReady(struct FileRequest *request);
void func_00288C50(struct FileRequest *request);
void func_002C81D0(struct FileRequest *request);

struct FileRequest *fileQueueWindowSlotRequest(const char *requestName, void *data, s32 size);

#endif /* FILE_REQUEST_API_H */
