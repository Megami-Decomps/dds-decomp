#ifndef FILE_REQUEST_API_H
#define FILE_REQUEST_API_H

#include "common.h"

struct FileRequest;
struct FilePacRequest;
struct sdfMemHandle_t;

/* Return the request's allocation descriptor without changing ownership. */
struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request);
/* Return the address of the loaded resource payload as a byte pointer. */
u8 *fileGetLoadPtr(const struct FileRequest *request);
/* Return the signed byte count recorded by the file transfer request. */
s32 fileGetLoadSize(const struct FileRequest *request);
/* Free a completed request wrapper; return one while it remains pending. */
s32 fileRequestFreeIfComplete(struct FileRequest *request);

/* Each file/PAC request copies requestName into storage it owns until cleanup.
 * The caller keeps ownership of its input string and may modify or release it
 * as soon as the call returns. */
#ifdef VERSION_DDS1
struct FileRequest *pacLoadEx(const char *requestName, u32 flags, void *packetCallbackAddress,
                                                u32 onComplete, u32 userData);
#else
struct FilePacRequest *fileCreatePacLoadWork(const char *requestName, s32 flags, void *dispatchValue,
                                             s32 onComplete, s32 userData);
#endif

/* fileLoadEx stores userData without copying it. When callbackAddress is
 * nonzero, the manager calls it once after the device request closes, passing
 * the completed request and userData. The context must remain valid until then;
 * callback delivery does not free the request wrapper, which callers release
 * with fileRequestFreeIfComplete. */
#ifdef VERSION_DDS1
struct FileRequest *fileLoadEx(const char *requestName, u32 allocationMode,
                                              u32 callbackAddress, u32 userData);
#else
struct FileRequest *fileLoadEx(const char *requestName, s32 allocationMode,
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

/* fileSave also copies requestName into request-owned storage. It separately
 * borrows sourceData for the asynchronous write, which must stay readable and
 * unchanged until CLOSED. */
struct FileRequest *fileSave(const char *requestName, const void *sourceData, s32 sourceByteCount);

#endif /* FILE_REQUEST_API_H */
