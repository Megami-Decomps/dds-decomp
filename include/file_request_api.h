#ifndef FILE_REQUEST_API_H
#define FILE_REQUEST_API_H

#include "common.h"

struct FileRequest;
struct FilePacRequest;
struct SdfMemBlock;

/* Return the request's allocation descriptor without changing ownership. */
struct SdfMemBlock *fileGetMemHandle(const struct FileRequest *request);
/* Return the address of the loaded resource payload as a byte pointer. */
u8 *fileGetLoadPtr(const struct FileRequest *request);
u32 fileGetLoadSize(const struct FileRequest *request);
s32 fileHandleFree(struct FileRequest *request);

#ifdef VERSION_DDS1
struct FileRequest *pacLoadEx(const char *requestName, u32 flags, u32 dispatchValue,
                                                u32 onComplete, u32 userData);
struct FileRequest *fileLoadEx(const char *requestName, u32 callbackMode,
                                              u32 callbackAddress, u32 userData);
#else
struct FilePacRequest *fileCreatePacLoadWork(const char *requestName, s32 flags, void *dispatchValue,
                                             s32 onComplete, s32 userData);
struct FileRequest *fileLoadEx(const char *requestName, s32 callbackMode,
                                              s32 callbackAddress, s32 userData);
#endif

struct FileRequest *pacLoad(const char *requestName);
void pacLoadUseTail(const char *requestName);
struct FileRequest *fileLoad(const char *requestName);
struct FileRequest *fileLoadUseTail(const char *requestName);

s32 pacFileCheckEnd(const struct FileRequest *request);
s32 pacCheckEnd(const struct FileRequest *request);
void fileSync(struct FileRequest *request);
void pacSync(struct FileRequest *request);
void sdfMcFuncFormat(u32 port);

struct FileRequest *fileSave(const char *requestName, const void *sourceData, s32 sourceByteCount);

#endif /* FILE_REQUEST_API_H */
