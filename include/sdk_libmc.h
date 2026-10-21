#ifndef SDK_LIBMC_H
#define SDK_LIBMC_H

/* SDK memory card library (libmc) and the game-named file-stream wrappers linked with it. One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

typedef struct SdkFileStream SdkFileStream;

#ifndef SDK_SKIP_sceMcChdir
s32 sceMcChdir(s32 port, s32 slot, const char *path, char *newPath);
#endif
#ifndef SDK_SKIP_sceMcDelete
s32 sceMcDelete(s32 port, s32 slot, const char *path);
#endif
#ifndef SDK_SKIP_sceMcMkdir
s32 sceMcMkdir(s32 port, s32 slot, const char *path);
#endif
#ifndef SDK_SKIP_sceMcOpen
s32 sceMcOpen(s32 port, s32 slot, const char *path, s32 mode);
#endif
#ifndef SDK_SKIP_sceMcSync
s32 sceMcSync(s32 mode, s32 *command, s32 *result);
#endif

#ifdef VERSION_DDS2
#ifndef SDK_SKIP_sdfAllocHeapMemory
void *sdfAllocHeapMemory(u32 size);
#endif
#ifndef SDK_SKIP_sdfCloseFileStream
void sdfCloseFileStream(SdkFileStream *stream);
#endif
#ifndef SDK_SKIP_sdfFreeHeapMemory
void sdfFreeHeapMemory(void *memory);
#endif
#ifndef SDK_SKIP_sdfOpenFileStream
SdkFileStream *sdfOpenFileStream(const char *name, const char *mode);
#endif
#ifndef SDK_SKIP_sdfReadFileStream
u32 sdfReadFileStream(void *data, u32 size, u32 count, SdkFileStream *stream);
#endif
#ifndef SDK_SKIP_sdfWriteFileStream
u32 sdfWriteFileStream(const void *data, u32 size, u32 count, SdkFileStream *stream);
#endif
#endif

#endif
