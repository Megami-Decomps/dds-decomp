#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
#include "sdf_file_existence_api.h"
#include "sdf_console_packet.h"
#include "sif_rpc_client_prefix.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_alarm.h"
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
#include "sdf_dev_start_load_api.h"
#include "sdf_disc_file_record.h"
#include "btl.h"
#include "sdf_debug_api.h"

extern SdfFileCommandQueueResult sdfFileCmdFileSize(sdfFileCmdHandle_t *state);

#define SDF_DEV_WORKER_COUNT 4

#define SDF_DEV_DEFAULT_PRIORITY 0x48

#define SDF_DEV_OVERRIDE_PRIORITY 0x78

#define SDF_DEV_PRIORITY_OVERRIDE_TICKS 3

#define SDF_DEV_FILE_OPEN_READ 1

#define SDF_DEV_FILE_OPEN_WRITE 0x202

#define SDF_DEV_SEEK_START 0

#define SDF_DEV_SEEK_CURRENT 1

#define SDF_DEV_SEEK_END 2

#define SDF_DEV_STATE_BYTES 0x40

#define SDF_DEV_INVALID_FILE_HANDLE -1

#define SDF_DEV_CACHE_EVICTION_THRESHOLD 9

extern u8 sdfFileCmdThreadPrioCheckCounter;

extern s32 sdfFileCmdThreadPrio;

extern DevWorkerEntry sdfFileDeviceCB[];

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path,
                                void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32), void *callbackContext);

extern SdfFileWorkerIndex sdfFileCmdAnalyzeFileName(const char *path, char **outResolvedPath);

extern void sdfFileCmdAllocHandle(sdfFileCmdHandle_t *state);

extern sdfFileCmdHandle_t *sdfFileCmdNewHandle(char *resolvedPath, SdfFileWorkerIndex workerIndex, s32 operation,
                                void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32), void *callbackContext);

extern u16 sdfFileCmdDefaultCreateMode;

extern void sdfCdImageClose(void);

s32 sdfCdImageGetCurrFileSize(void);

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */ s32 sdfCdImageRead(void *buffer, s32 byteCount);

s32 sdfDevOpenDiscFileAndGetSize(const char *name);

extern s32 sdfFileCmdSeek(sdfFileCmdHandle_t *state, s32 seekOffset, s32 seekOrigin);

extern u8 D_00438B1E;

extern void sdfFileCmdStartThread(s32 index);

extern sdfFileCmdHandle_t *D_00438B08;

extern sdfFileCmdHandle_t *D_00438B0C;

extern s16 D_00438B1C;

char *sdfFileGetHostRoot(void);

extern sdfFileCmdHandle_t *D_00438B14;

extern sdfFileCmdHandle_t *D_00438B18;

extern s16 D_00438B10;

void sdfFileCmdUnlink(sdfFileCmdHandle_t *state) {
    sdfFileCmdHandle_t *prev;
    sdfFileCmdHandle_t *next;
    s32 interrupts;

    interrupts = DIntr();
    prev = state->previous;
    next = state->next;
    if (prev == NULL) {
        D_00438B14 = next;
    } else {
        prev->next = next;
    }
    if (next == NULL) {
        D_00438B18 = prev;
    } else {
        next->previous = prev;
    }
    D_00438B10 -= 1;
    if (interrupts != 0) {
        EIntr();
    }
    sdfChipFreeDirect(state->resolvedPath);
    sdfChipFreeDirect(state);
}

/* Move state from both active lists to the completed cache, evict the oldest
 * entry at nine cached states, and wake any remaining request on this worker. */
void sdfFileCmdDequeue(sdfFileCmdHandle_t *state) {
    DevWorkerEntry *worker;
    sdfFileCmdHandle_t *prev;
    sdfFileCmdHandle_t *next;
    s32 interrupts;

    interrupts = DIntr();
    prev = state->previous;
    next = state->next;
    if (prev == NULL) {
        D_00438B08 = next;
    } else {
        prev->next = next;
    }
    if (next == NULL) {
        D_00438B0C = prev;
    } else {
        next->previous = prev;
    }
    worker = &sdfFileDeviceCB[state->workerIndex];
    prev = state->workerPrev;
    next = state->workerNext;
    if (prev == NULL) {
        worker->first = next;
    } else {
        prev->workerNext = next;
    }
    if (next == NULL) {
        worker->last = prev;
    } else {
        next->workerPrev = prev;
    }
    state->previous = D_00438B18;
    if (D_00438B18 == NULL) {
        D_00438B14 = state;
    } else {
        D_00438B18->next = state;
    }
    D_00438B18 = state;
    state->next = NULL;
    D_00438B10++;
    D_00438B1C--;
    if (interrupts != 0) {
        EIntr();
    }
    if (D_00438B10 >= SDF_DEV_CACHE_EVICTION_THRESHOLD) {
        sdfFileCmdUnlink(D_00438B14);
    }
    if (worker->first != NULL) {
        SignalSema(worker->semaphore);
    }
}

/* Close a valid file handle, invalidate it, and retain the completed state in the cache. */
void sdfFileCmdReleaseHandle(sdfFileCmdHandle_t *state) {
    s32 fileHandle;

    fileHandle = state->fileHandle;
    state->fileHandle = SDF_DEV_INVALID_FILE_HANDLE;
    if (fileHandle >= 0) {
        sceClose(fileHandle);
    }
    sdfFileCmdDequeue(state);
}

/* Store an error/result, release the request, and notify its callback of inactivity. */
void sdfFileCmdFinish(sdfFileCmdHandle_t *state, s32 result) {
    state->result = result;
    state->state = SDF_DEV_STATE_INACTIVE;
    sdfFileCmdReleaseHandle(state);
    if (state->callback != NULL) {
        state->callback(state, SDF_DEV_EVENT_INACTIVE, 0, 0, (s32)state->callbackContext);
    }
}

void sdfFileCmdThread(DevWorkerEntry *worker) {
    sdfFileCmdHandle_t *state;
    void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32);
    s32 fileHandle;
    s32 result;
    s32 position;
    s32 transferResult;
    s32 endOffset;
    void *buffer;
    u32 operationIndex;

    for (;;) {
        WaitSema(worker->semaphore);
        state = worker->first;
        if (state == NULL) {
            continue;
        }

        operationIndex = state->operation - SDF_DEV_OPERATION_OPEN_READ;
        state->operation = SDF_DEV_OPERATION_NONE;
        switch (operationIndex) {
        case SDF_DEV_OPERATION_OPEN_READ - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = sceOpen(state->resolvedPath, SDF_DEV_FILE_OPEN_READ, 0);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->fileHandle = fileHandle;
            state->transferred = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_OPENED, 0, 0, (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_OPEN_WRITE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = sceOpen(state->resolvedPath, SDF_DEV_FILE_OPEN_WRITE, state->options);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->fileHandle = fileHandle;
            state->transferred = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_OPENED, 0, 0, (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_SEEK - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_SEEKING;
            endOffset = sceLseek(state->fileHandle, state->seekOffset, state->options);
            if (endOffset < 0) {
                sdfFileCmdFinish(state, endOffset);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred = endOffset;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_SEEK_REPLY, 0, endOffset, (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_GET_SIZE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_SEEKING;
            position = sceLseek(state->fileHandle, 0, SDF_DEV_SEEK_CURRENT);
            if (position < 0) {
                sdfFileCmdFinish(state, position);
                continue;
            }
            endOffset = sceLseek(state->fileHandle, 0, SDF_DEV_SEEK_END);
            if (endOffset < 0) {
                sdfFileCmdFinish(state, endOffset);
                continue;
            }
            result = sceLseek(state->fileHandle, position, SDF_DEV_SEEK_START);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred = endOffset;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_SIZE_REPLY, 0, endOffset, (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_READ - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_READING;
            result = sceRead(state->fileHandle, (s32)state->requestData,
                           state->requestExtra);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_READ_REPLY, (s32)state->requestData, result,
                                (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_WRITE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_WRITING;
            result = sceWrite(state->fileHandle, (s32)state->requestData,
                            state->requestExtra);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_WRITE_REPLY, (s32)state->requestData, result,
                                (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_CLOSE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_CLOSING;
            result = sceClose(state->fileHandle);
            state->fileHandle = SDF_DEV_INVALID_FILE_HANDLE;
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            sdfFileCmdDequeue(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_CLOSED, 0, 0, (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_READ_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = sceOpen(state->resolvedPath, SDF_DEV_FILE_OPEN_READ, 0);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->fileHandle = fileHandle;
            transferResult = state->requestExtra;
            /* A negative count means the whole file; NULL storage is allocated below. */
            if (transferResult < 0) {
                state->state = SDF_DEV_STATE_SEEKING;
                transferResult = sceLseek(fileHandle, 0, SDF_DEV_SEEK_END);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
                result = sceLseek(state->fileHandle, 0, SDF_DEV_SEEK_START);
                if (result < 0) {
                    sdfFileCmdFinish(state, result);
                    continue;
                }
            }
            if (transferResult != 0) {
                buffer = state->requestData;
                if (buffer == NULL) {
                    buffer = (void *)sdfMemLock((struct sdfMemHandle_t *)(sdfMemAllocDirect(transferResult)));
                }
                state->requestData = buffer;
                state->state = SDF_DEV_STATE_READING;
                transferResult = sceRead(state->fileHandle, (s32)buffer, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            state->state = SDF_DEV_STATE_CLOSING;
            sceClose(state->fileHandle);
            state->fileHandle = SDF_DEV_INVALID_FILE_HANDLE;
            sdfFileCmdDequeue(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_BUFFER_REPLY, (s32)state->requestData, transferResult,
                         (s32)state->callbackContext);
            }
            SignalSema(worker->semaphore);
            continue;

        case SDF_DEV_OPERATION_WRITE_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = sceOpen(state->resolvedPath, SDF_DEV_FILE_OPEN_WRITE, state->options);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->fileHandle = fileHandle;
            transferResult = state->requestExtra;
            if (transferResult != 0) {
                state->state = SDF_DEV_STATE_WRITING;
                transferResult = sceWrite(fileHandle, (s32)state->requestData, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            break;

        default:
            continue;
        }

        state->state = SDF_DEV_STATE_CLOSING;
        sceClose(state->fileHandle);
        state->fileHandle = SDF_DEV_INVALID_FILE_HANDLE;
        sdfFileCmdDequeue(state);
        state->state = SDF_DEV_STATE_COMPLETE;
        callback = state->callback;
        if (callback != NULL) {
            callback(state, SDF_DEV_EVENT_BUFFER_REPLY, (s32)state->requestData, transferResult,
                            (s32)state->callbackContext);
        }
        SignalSema(worker->semaphore);
    }
}

/* Worker slot 3 services disc-backed requests through serialized disc operations. */
void sdfFileCdImageCmdThread(DevWorkerEntry *worker) {
    sdfFileCmdHandle_t *state;
    void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32);
    s32 result;
    s32 transferResult;
    void *readBuffer;
    u32 operation;

    for (;;) {
        WaitSema(worker->semaphore);
        state = worker->first;
        if (state == NULL) {
            continue;
        }

        operation = state->operation - SDF_DEV_OPERATION_OPEN_READ;
        state->operation = SDF_DEV_OPERATION_NONE;
        switch (operation) {
        case SDF_DEV_OPERATION_OPEN_READ - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            result = sdfDevOpenDiscFileAndGetSize(state->resolvedPath);
            if (result < 0) {
                sdfFileCmdFinish(state, 0);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_OPENED, 0, 0, (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_GET_SIZE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_ACTIVE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_SIZE_REPLY, 0, sdfCdImageGetCurrFileSize(),
                                (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_READ - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_READING;
            result = sdfCdImageRead(state->requestData, state->requestExtra);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_READ_REPLY, (s32)state->requestData, result,
                                (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_CLOSE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_CLOSING;
            sdfCdImageClose();
            sdfFileCmdDequeue(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_CLOSED, 0, 0, (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_READ_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            result = sdfDevOpenDiscFileAndGetSize(state->resolvedPath);
            if (result < 0) {
                sdfFileCmdFinish(state, 0);
                continue;
            }
            transferResult = state->requestExtra;
            if (transferResult < 0) {
                transferResult = sdfCdImageGetCurrFileSize();
            }
            if (transferResult != 0) {
                readBuffer = state->requestData;
                if (readBuffer == NULL) {
                    readBuffer = (void *)sdfMemLock((struct sdfMemHandle_t *)(sdfMemAllocDirect(transferResult)));
                }
                state->requestData = readBuffer;
                state->state = SDF_DEV_STATE_READING;
                transferResult = sdfCdImageRead(readBuffer, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            state->state = SDF_DEV_STATE_CLOSING;
            sdfCdImageClose();
            state->fileHandle = -1;
            sdfFileCmdDequeue(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_BUFFER_REPLY, (s32)state->requestData, transferResult,
                                (s32)state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_OPEN_WRITE - SDF_DEV_OPERATION_OPEN_READ:
        case SDF_DEV_OPERATION_SEEK - SDF_DEV_OPERATION_OPEN_READ:
        case SDF_DEV_OPERATION_WRITE - SDF_DEV_OPERATION_OPEN_READ:
        case SDF_DEV_OPERATION_WRITE_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            sdfFileCmdFinish(state, 0);
            continue;

        default:
            continue;
        }
    }
}

extern char D_00438B28[]; /* cdrom0: */

extern char D_00438B30[]; /* host0: */

extern char D_00438B38[]; /* pfs0: */

/* Allocate a worker-specific device path; an unsupported worker returns NULL. */
char *sdfFileCmdMakeRealFileName(const char *path, SdfFileWorkerIndex worker) {
    s32 length = strlen(path);
    char *result;
    s32 index;
    s32 character;
    char *base;
    s32 baseLength;
    switch (worker) {
    case 0:
        result = sdfChipAllocDirect(length + 10);
        memcpy(result, D_00438B28, 7);
        index = 0;
        character = path[index];
        while (character != 0) {
            if (character == '/') character = '\\';
            else if (character >= 'a' && character <= 'z') character -= 0x20;
            result[index + 7] = character;
            index++;
            character = path[index];
        }
        result[index + 7] = ';';
        result[index + 8] = '1';
        result[index + 9] = 0;
        break;
    case 1:
        if (path[0] != '/') {
            result = sdfChipAllocDirect(length + 7);
            memcpy(result, D_00438B30, 6);
            memcpy(result + 6, path, length);
            result[length + 6] = 0;
        } else {
            base = sdfFileGetHostRoot();
            baseLength = strlen(base);
            result = sdfChipAllocDirect(length + baseLength);
            memcpy(result, base, baseLength);
            memcpy(result + baseLength, path + 1, length);
        }
        break;
    case 2:
        result = sdfChipAllocDirect(length + 6);
        memcpy(result, D_00438B38, 5);
        memcpy(result + 5, path, length);
        result[length + 5] = 0;
        break;
    case 3:
        result = sdfChipAllocDirect(length + 1);
        strcpy(result, path);
        break;
    default:
        return NULL;
    }
    return result;
}

extern char D_00438B40[]; /* /movie/ */

extern char D_00438B48[]; /* cdimg: */

/* Split the device prefix off a path and report the worker that serves it. */
SdfFileWorkerIndex sdfFileCmdAnalyzeFileName(const char *path, char **resolvedPath) {
    SdfFileWorkerIndex worker;
    s32 prefixLength;
    const char *suffix;
    const char *name;

    if (strchr(path, ':') != NULL) {
        if (memcmp(path, D_00438B28, 7) == 0) {
            suffix = path + 7;
            if (memcmp(suffix, D_00438B40, 7) == 0) {
                worker = 3;
                prefixLength = 0;
            } else if (memcmp(suffix, "/movie2/", 7) == 0) {
                worker = 3;
                prefixLength = 0;
            } else {
                worker = 0;
                prefixLength = 7;
            }
        } else if (memcmp(path, D_00438B30, 6) == 0) {
            worker = 1;
            prefixLength = 6;
        } else if (memcmp(path, D_00438B38, 5) == 0) {
            worker = 2;
            prefixLength = 5;
        } else if (memcmp(path, D_00438B48, 6) == 0) {
            worker = 3;
            prefixLength = 6;
        } else {
            return -1;
        }
        name = path + prefixLength;
    } else {
        worker = D_00438B1E;
        name = path;
    }
    *resolvedPath = sdfFileCmdMakeRealFileName(name, worker);
    return worker;
}

/* Allocate a request state and retain the callback's opaque context pointer. */
sdfFileCmdHandle_t *sdfFileCmdNewHandle(char *resolvedPath, SdfFileWorkerIndex workerIndex, s32 operation,
                        void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32), void *callbackContext) {
    sdfFileCmdHandle_t *state = (sdfFileCmdHandle_t *)sdfChipAllocZeroDirect(SDF_DEV_STATE_BYTES);
    state->resolvedPath = resolvedPath;
    state->workerIndex = workerIndex;
    state->operation = operation;
    state->state = SDF_DEV_STATE_QUEUED;
    state->fileHandle = SDF_DEV_INVALID_FILE_HANDLE;
    state->callback = callback;
    state->callbackContext = callbackContext;
    return state;
}

/* Resolve a path to its worker and enqueue an asynchronous read-only open. */
sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32),
                        void *callbackContext) {
    char *resolvedPath;
    SdfFileWorkerIndex workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_OPEN_READ, callback, callbackContext);
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Enqueue a write open; zero options select the device's default creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32),
                        void *callbackContext, s32 options) {
    char *resolvedPath;
    SdfFileWorkerIndex workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_OPEN_WRITE, callback, callbackContext);
    state->options = options != 0 ? options : sdfFileCmdDefaultCreateMode;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Queue a seek with its offset and origin; return -1 unless the request is active. */
s32 sdfFileCmdSeek(sdfFileCmdHandle_t *state, s32 seekOffset, s32 seekOrigin) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->seekOffset = seekOffset;
    state->options = seekOrigin;
    state->operation = SDF_DEV_OPERATION_SEEK;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return 0;
}

/* Queue a size query for an active request and report queue acceptance. */
SdfFileCommandQueueResult sdfFileCmdFileSize(sdfFileCmdHandle_t *state) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return SDF_FILE_COMMAND_QUEUE_RESULT_NOT_QUEUED;
    }
    state->operation = SDF_DEV_OPERATION_GET_SIZE;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return SDF_FILE_COMMAND_QUEUE_RESULT_QUEUED;
}

/* Queue a byte-counted read and report queue acceptance. */
SdfFileCommandQueueResult sdfFileCmdRead(sdfFileCmdHandle_t *state, void *readBuffer, s32 byteCount) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return SDF_FILE_COMMAND_QUEUE_RESULT_NOT_QUEUED;
    }
    state->requestExtra = byteCount;
    state->operation = SDF_DEV_OPERATION_READ;
    state->requestData = readBuffer;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return SDF_FILE_COMMAND_QUEUE_RESULT_QUEUED;
}

/* Queue a byte-counted write and report queue acceptance. */
SdfFileCommandQueueResult sdfFileCmdWrite(sdfFileCmdHandle_t *state, const void *buffer, s32 byteCount) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return SDF_FILE_COMMAND_QUEUE_RESULT_NOT_QUEUED;
    }
    state->requestExtra = byteCount;
    state->operation = SDF_DEV_OPERATION_WRITE;
    state->requestData = (void *)buffer;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return SDF_FILE_COMMAND_QUEUE_RESULT_QUEUED;
}

/* Clear an inactive request's result and return it to the active state. */
s32 sdfFileCmdClearError(sdfFileCmdHandle_t *state) {
    if (state->state != SDF_DEV_STATE_INACTIVE) {
        return -1;
    }
    state->result = 0;
    state->state = SDF_DEV_STATE_ACTIVE;
    return 0;
}

/* Queue a close for an active request. */
s32 sdfFileCmdClose(sdfFileCmdHandle_t *request) {
    SdfDevStateCode requestState = request->state;

    if (requestState != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    request->operation = SDF_DEV_OPERATION_CLOSE;
    SignalSema(sdfFileDeviceCB[request->workerIndex].semaphore);
    return 0;
}

/* Free a retained active/completed state; reject other states with -1. */
s32 sdfFileCmdFree(sdfFileCmdHandle_t *state) {
    if (state->state < SDF_DEV_STATE_INACTIVE) {
        if (state->state >= SDF_DEV_STATE_ACTIVE) {
            sdfFileCmdUnlink(state);
            return 0;
        }
    }
    return -1;
}

/* Enqueue a one-shot read: negative byte count requests the whole file. */
sdfFileCmdHandle_t *sdfFileCmdReadFile(const char *path, void *buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32), void *callbackContext) {
    char *resolvedPath;
    SdfFileWorkerIndex workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_READ_BUFFER, callback, callbackContext);
    state->requestExtra = byteCount;
    state->requestData = buffer;
    state->seekOffset = 0;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Enqueue a one-shot write using the supplied buffer, byte count and creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteFile(const char *path, void *buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, s32),
                        void *callbackContext, s32 options) {
    char *resolvedPath;
    SdfFileWorkerIndex workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_WRITE_BUFFER, callback, callbackContext);
    state->requestExtra = byteCount;
    state->requestData = buffer;
    state->options = options != 0 ? options : sdfFileCmdDefaultCreateMode;
    state->seekOffset = 0;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Update non-negative worker IDs only; skip an unchanged numeric priority. */
void sdfFileCmdThreadSetPrio(s32 priority) {
    DevWorkerEntry *worker;
    u32 index;

    if (sdfFileCmdThreadPrio == priority) {
        return;
    }
    sdfFileCmdThreadPrio = priority;
    worker = sdfFileDeviceCB;
    index = 0;
    do {
        s32 threadId = worker->threadId;

        worker++;
        if (threadId >= 0) {
            ChangeThreadPriority(threadId, priority);
        }
        index++;
    } while (index < SDF_DEV_WORKER_COUNT);
}

/* Apply the temporary numeric priority for three countdown ticks. */
void sdfFileCmdThreadSetLoPrio(void) {
    sdfFileCmdThreadPrioCheckCounter = SDF_DEV_PRIORITY_OVERRIDE_TICKS;
    sdfFileCmdThreadSetPrio(SDF_DEV_OVERRIDE_PRIORITY);
}

/* Restore the default numeric priority without changing the countdown. */
void sdfFileCmdThreadSetHiPrio(void) {
    sdfFileCmdThreadSetPrio(SDF_DEV_DEFAULT_PRIORITY);
}

/* Decrement an active override and restore the default when it expires. */
void sdfFileCmdThreadCheckPrio(void) {
    u8 remainingTicks = sdfFileCmdThreadPrioCheckCounter;
    u8 nextTicks;

    if (remainingTicks == 0) {
        return;
    }
    sdfFileCmdThreadPrioCheckCounter = remainingTicks - 1;
    nextTicks = remainingTicks - 1;
    if (nextTicks != 0) {
        return;
    }
    sdfFileCmdThreadSetHiPrio();
}

/* Start worker thread `index` if it isn't running; slot 3 runs the alternate entry point. */
void sdfFileCmdStartThread(s32 index) {
    DevWorkerEntry *worker = &sdfFileDeviceCB[index];
    void (*entry)();
    s32 thread;

    thread = worker->threadId;
    if (thread < 0) {
        worker->semaphore = sdfCreateSema(0, 0xFF, 0);
        entry = sdfFileCdImageCmdThread;
        if (index != 3) {
            entry = sdfFileCmdThread;
        }
        sdfFileCmdThreadPrio = 0x48;
        thread = sdfCreateThreadWithStack(entry, 0x4000, 0x48);
        worker->threadId = thread;
        _StartThread(thread, (s32)worker);
    }
}
INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B10);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B14);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B18);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B1C);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B1E);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdThreadPrioCheckCounter);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdThreadPrio);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdDefaultCreateMode);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B28);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B30);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B38);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B40);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", D_00438B48);

