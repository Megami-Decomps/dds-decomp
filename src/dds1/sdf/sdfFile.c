#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
#include "sdf_file_existence_api.h"
#include "sdf_console_packet.h"
#include "sif_rpc_client_prefix.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "sdf_timer.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf_draw.h"
#include "sdf_packet_alloc.h"
#include "sdf_sif_command.h"
#include "sdf_dev_event.h"
#include "sdf_dev_protocol.h"
#include "sdf_dev_state.h"
#include "sdf_dev_path_prefix.h"
#include "sdf_dev_worker.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_dev_start_load_api.h"
#include "sdf_disc_file_record.h"
#include "btl.h"
#include "sdf_debug_api.h"

extern SdfFileCommandQueueResult sdfFileCmdRequestFileSize(sdfFileCmdHandle_t *state);

#define SDF_DEV_REPLY_SEMAPHORE_LIMIT 0x80

extern u8 D_003BD42E;

extern u8 D_003BD3F0;

extern u32 sdfFileSeekWork;

extern u32 sdfFileLengthWork;

extern s32 sdfFileSemaId;

extern void sdfDevSetFallbackDeviceWorker(s32 worker);

extern char sdfFileHostRootPath[];

extern DevWorkerEntry sdfFileDeviceCB[];

extern u8 sdfPfsPathPrefix[];

extern char *sdfFileMakeFilename(const char *path);

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path,
                                void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32), void *callbackContext);

extern void sdfFileCmdAllocHandle(sdfFileCmdHandle_t *state);

s32 sdfCdImageCheckExist(const char *path);

extern char sdfDiscPathPrefix[];

/* Convert a relative disc path to the drive's uppercase backslash form with ;1 suffix. */
void sdfFileSetCdFilename(char *dst, const char *src) {
    s32 c;
    memcpy(dst, sdfDiscPathPrefix, 8);
    dst += 7;
    c = *src++;
    while (c != 0) {
        if (c == '/') {
            c = '\\';
        }
        if (c >= 'a' && c <= 'z') {
            c -= 0x20;
        }
        *dst++ = c;
        c = *src++;
    }
    dst[0] = ';';
    dst[1] = '1';
    dst[2] = 0;
}

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_003BD400[];

char *sdfFileSetHostFilename(char *dst, const char *src) {
    if (*src == 0x2F) {
        strcpy(dst, sdfFileHostRootPath);
        return strcat(dst, src);
    }
    *(Bytes7 *)dst = D_003BD400[0];
    return strcat(dst, src);
}

void sdfFileSetHddFilename(char *destination, const char *path) {
    memcpy(destination, sdfPfsPathPrefix, 6);
    strcat(destination, path);
}

/* Retail disables the implicit-disc path probe used by the ordered device dispatch. */
u32 sdfFileChkCdFile(const char *path) {
    return 0;
}

/* Retail disables the implicit-host probe; the host builder is still used explicitly. */
u32 sdfFileChkHostFile(const char *path) {
    return 0;
}

/* Retail disables the implicit-PFS probe before the configured-device fallback. */
u32 sdfFileChkHddFile(const char *path) {
    return 0;
}

extern char D_003BD410[]; 

/* Return an allocated device path, normalizing disc-path case and separators. */
char *sdfFileMakeFilename(const char *path) {
    s32 length = strlen(path);
    char *result = sdfChipAllocDirect(0x80);
    char *cursor;
    s32 character;
    if (strchr(path, ':') != NULL) {
        memcpy(result, path, length);
        result[length] = 0;
        if (strncmp(result, D_003BD410, 5) == 0) {
            cursor = result + 5;
            character = *cursor;
            while (character != 0) {
                if (character == '/') *cursor = '\\';
                else if (character >= 'a' && character <= 'z') *cursor = character - 0x20;
                cursor++;
                character = *cursor;
            }
            *cursor++ = ';';
            *cursor++ = '1';
            *cursor = 0;
        }
    } else if (sdfFileChkCdFile(path)) {
        sdfFileSetCdFilename(result, path);
    } else if (sdfFileChkHostFile(path)) {
        sdfFileSetHostFilename(result, path);
    } else if (sdfFileChkHddFile(path)) {
        sdfFileSetHddFilename(result, path);
    } else {
        switch (D_003BD3F0) {
        case 0: sdfFileSetCdFilename(result, path); break;
        case 2: sdfFileSetHddFilename(result, path); break;
        case 1: sdfFileSetHostFilename(result, path); break;
        default: strcpy(result, path); break;
        }
    }
    return result;
}

/* Capture seek/size replies, then wake the synchronous command waiter. */
void sdfFileCallback(sdfFileCmdHandle_t *state, SdfDevEvent event, s32 unused, s32 value, s32 callbackContext) {
    if (event != SDF_DEV_EVENT_SEEK_REPLY) {
        if (event == SDF_DEV_EVENT_SIZE_REPLY) {
            sdfFileLengthWork = value;
        }
    } else {
        sdfFileSeekWork = value;
    }
    SignalSema(sdfFileSemaId);
}

/* Open a path using the shared reply callback and wait for its initial completion. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path) {
    sdfFileCmdHandle_t *state;
    if (sdfFileSemaId < 0) {
        sdfFileSemaId = sdfCreateSema(0, SDF_DEV_REPLY_SEMAPHORE_LIMIT, 0);
    }
    state = sdfFileCmdReadOpen(path, sdfFileCallback, 0);
    WaitSema(sdfFileSemaId);
    sdfFileCmdClearError(state);
    return state;
}

SdfFileExistenceResult sdfFileCheckExist(const char *path) {
    char *resolved = sdfFileMakeFilename(path);
    s32 fd;

    if (*resolved == '/') {
        return sdfCdImageCheckExist(resolved);
    }
    fd = sceOpen(resolved, 1);
    sceClose(fd);
    sdfChipFreeDirect(resolved);
    return fd >= 0;
}

void sdfFileClose(sdfFileCmdHandle_t *state) {
    sdfFileCmdClose(state);
    WaitSema(sdfFileSemaId);
    sdfFileCmdFree(state);
}

/* Queue a byte-counted read, then wait for its reply callback. */
void sdfFileRead(sdfFileCmdHandle_t *state, void *buffer, s32 byteCount) {
    sdfFileCmdRead(state, buffer, byteCount);
    WaitSema(sdfFileSemaId);
}

u32 sdfFileLength(sdfFileCmdHandle_t *state) {
    sdfFileCmdRequestFileSize(state);
    WaitSema(sdfFileSemaId);
    return sdfFileLengthWork;
}

extern s32 sdfFileCmdSeek(sdfFileCmdHandle_t *state, s32 seekOffset, s32 seekOrigin);

u32 sdfFileSeek(sdfFileCmdHandle_t *state, s32 seekOffset, s32 seekOrigin) {
    sdfFileCmdSeek(state, seekOffset, seekOrigin);
    WaitSema(sdfFileSemaId);
    return sdfFileSeekWork;
}

/* Set the current device directory, ending in a slash; path is assumed nonempty. */
void sdfFileSetHostRoot(char *path) {
    s32 length = strlen(path);

    ((DevPathPrefix *)sdfFileHostRootPath)->head = ((DevPathPrefix *)D_003BD400)->head;
    ((DevPathPrefix *)sdfFileHostRootPath)->tail = ((DevPathPrefix *)D_003BD400)->tail;
    memcpy(sdfFileHostRootPath + 6, path, length);
    if (path[length - 1] != '/') {
        sdfFileHostRootPath[length + 6] = '/';
        sdfFileHostRootPath[length + 7] = 0;
    } else {
        sdfFileHostRootPath[length + 6] = 0;
    }
}

char *sdfFileGetHostRoot(void) {
    return sdfFileHostRootPath;
}

/* Select the default path-construction worker and the no-prefix fallback worker. */
void sdfFileSetDefaultSrc(s32 worker) {
    D_003BD3F0 = worker;
    sdfDevSetFallbackDeviceWorker(worker);
}

/* Select the worker used when a file path has no explicit device prefix. */
void sdfDevSetFallbackDeviceWorker(s32 worker) {
    D_003BD42E = worker;
}

extern sdfFileCmdHandle_t *D_003BD418;

extern sdfFileCmdHandle_t *D_003BD41C;

extern s16 D_003BD42C;

void sdfFileCmdStartThread(s32 index);

/* Queue a request on the global list and on its worker's list; wake the worker when its list was empty. */
void sdfFileCmdAllocHandle(sdfFileCmdHandle_t *state) {
    DevWorkerEntry *worker;
    s32 interrupts;
    s32 wake = 0;
    sdfFileCmdHandle_t *last;

    interrupts = DIntr(state);
    worker = &sdfFileDeviceCB[state->workerIndex];
    if (worker->threadId < 0) {
        sdfFileCmdStartThread(state->workerIndex);
    }
    state->previous = D_003BD41C;
    if (D_003BD41C != NULL) {
        D_003BD41C->next = state;
    } else {
        D_003BD418 = state;
    }
    last = worker->last;
    D_003BD41C = state;
    state->workerPrev = last;
    if (last != NULL) {
        last->workerNext = state;
    } else {
        worker->first = state;
        wake = 1;
    }
    worker->last = state;
    D_003BD42C += 1;
    if (interrupts != 0) {
        EIntr();
    }
    if (wake != 0) {
        SignalSema(worker->semaphore);
    }
}

/* Writable device directory; its suffix is replaced by the path setter. */
char sdfFileHostRootPath[64] = "host0:";
INCLUDE_SDATA(const s32, "sdf/sdfFile", D_003BD3F0);

INCLUDE_SDATA(const s32, "sdf/sdfFile", sdfFileSemaId);

INCLUDE_SDATA(const s32, "sdf/sdfFile", sdfDiscPathPrefix);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_003BD400);

INCLUDE_SDATA(const s32, "sdf/sdfFile", sdfPfsPathPrefix);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_003BD410);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_003BD418);

INCLUDE_SDATA(const s32, "sdf/sdfFile", D_003BD41C);

