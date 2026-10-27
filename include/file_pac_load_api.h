#ifndef FILE_PAC_LOAD_API_H
#define FILE_PAC_LOAD_API_H

#include "common.h"

/* PAC payloads use the default allocator or the high-address tail allocator. */
#ifdef VERSION_DDS1
typedef u32 FilePacResourceAllocationMode;
#else
typedef s32 FilePacResourceAllocationMode;
#endif

enum {
    FILE_PAC_RESOURCE_ALLOCATION_DEFAULT = 0,
    FILE_PAC_RESOURCE_ALLOCATION_FROM_TAIL = 1
};

struct FileRequest;
struct FilePacRequest;

/* PAC loading copies requestName into storage owned until request cleanup. The
 * caller keeps ownership of its input string and may modify or release it as
 * soon as the call returns. */
#ifdef VERSION_DDS1
struct FileRequest *pacLoadEx(const char *requestName,
                              FilePacResourceAllocationMode allocationMode,
                              void *packetCallbackAddress, u32 onComplete,
                              u32 userData);
#else
struct FilePacRequest *fileCreatePacLoadWork(
    const char *requestName, FilePacResourceAllocationMode allocationMode,
    void *packetCallbackAddress, s32 onComplete, s32 userData);
#endif

#endif /* FILE_PAC_LOAD_API_H */
