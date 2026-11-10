#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

s32 strlen(const char *aStr);

s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

s32 sdfFileCmdClose(void *aHandle);

s32 sdfFileCmdFree(sdfFileCmd_t *aHandle);

s32 sdfFileCmdFileSize(void *aHandle);

s32 sdfFileCmdSeek(void *aHandle, s32 aOffset, s32 aWhence);

extern s32 WaitSema(s32 aSemaId);

extern s32 SignalSema(s32 aSemaId);

extern u8 sdfFileCmdThreadPrioCheckCounter;

void sdfFileCmdThreadSetPrio(s32 prio);

s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength);


void sdfFileCmdUnlink(void *aHandle);

void sdfFileCmdDequeue(void *aCmd);

s32 sceClose(s32 a);

typedef u8 sdfDevOperationCode_t;

typedef s8 sdfDevStateCode_t;

/* Complete 0x40-byte request owner. sdfFileCmd_t describes only its opaque
 * 0x30-byte prefix for consumers outside the device queue implementation. */
typedef struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next;
    struct sdfFileCmdHandle_t *prev;
    struct sdfFileCmdHandle_t *deviceNext;
    struct sdfFileCmdHandle_t *devicePrev;
    char *filename;
    u8 device;
    sdfDevOperationCode_t cmd;
    sdfDevStateCode_t stat;
    u8 phase;
    s32 offset;
    s32 length;
    void *buffer;
    s32 cmdParm;
    s32 fd;
    s32 errorCode;
    s32 currentOffset;
    u8 remainder[4];
    void (*callback)(struct sdfFileCmdHandle_t *, s32, s32, s32, s32);
    s32 userData;
} sdfFileCmdHandle_t;

typedef struct sdfFileDeviceCB_t {
    s32 thId;
    s32 semaId;
    struct sdfFileCmdHandle_t *first;
    struct sdfFileCmdHandle_t *last;
    u8 opaque10[8];
} sdfFileDeviceCB_t;

enum SdfFileIOCmdStat {
    SDF_FILE_CMD_STAT_INIT = 0,
    SDF_FILE_CMD_STAT_DORMANT = 1,
    SDF_FILE_CMD_STAT_OPEN = 2,
    SDF_FILE_CMD_STAT_SEEK = 3,
    SDF_FILE_CMD_STAT_READ = 4,
    SDF_FILE_CMD_STAT_WRITE = 5,
    SDF_FILE_CMD_STAT_CLOSE = 6,
    SDF_FILE_CMD_STAT_SUSPEND = 7,
    SDF_FILE_CMD_STAT_DONE = 8,
    SDF_FILE_CMD_STAT_ERROR = 9,
    SDF_FILE_CMD_STAT_CANCEL = 10
};

enum SdfFileIOCmd {
    SDF_FILE_CMD_NONE = 0,
    SDF_FILE_CMD_READOPEN = 1,
    SDF_FILE_CMD_WRITEOPEN = 2,
    SDF_FILE_CMD_SEEK = 3,
    SDF_FILE_CMD_FILESIZE = 4,
    SDF_FILE_CMD_READ = 5,
    SDF_FILE_CMD_WRITE = 6,
    SDF_FILE_CMD_CLOSE = 7,
    SDF_FILE_CMD_READFILE = 8,
    SDF_FILE_CMD_WRITEFILE = 9
};

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

#define SDK_SKIP_EIntr
#include "sdk_libkernl.h"

extern void *memcpy(void *, const void *, unsigned int);


extern s32 sceLseek(s32 fileHandle, s32 offset, s32 origin);

extern s32 sceRead(s32 fileHandle, s32 buffer, s32 byteCount);

extern char * strcpy(char *, const char *);

void *sdfChipAllocDirect(s32 aSize);

extern sdfFileDeviceCB_t sdfFileDeviceCB[];

extern s32 DIntr(void);

extern void EIntr(void);

extern void sdfChipFreeDirect(void *aPtr);

#define D_00551948 sdfFileCmdActiveHead
#define D_00551954 sdfFileCmdCacheHead
#define D_00551958 sdfFileCmdCacheTail
#define D_00551968 sdfFileCmdCdromPrefix
#define D_00551970 sdfFileCmdHostPrefix
#define D_00551978 sdfFileCmdPfsPrefix
#define D_00551980 sdfFileCmdMoviePath
#define D_00551988 sdfFileCmdCdimgPrefix

extern sdfFileCmdHandle_t *D_00551948;

extern sdfFileCmdHandle_t *sdfFileCmdFreeList;

extern s16 sdfFileCmdCount;

extern sdfFileCmdHandle_t *D_00551954;

extern sdfFileCmdHandle_t *D_00551958;

extern s16 sdfFileCmdFreeCount;

u32 sdfMemLock(sdfMemHandle_t *aHandle);

extern char D_00551968[];

extern char D_00551970[];

extern char D_00551978[];

extern char D_00551980[];

extern char D_00551988[];

extern u8 sdfFileCmdDefaultDevice;

extern s32 memcmp(const void *aFirst, const void *aSecond, u32 aSize);

extern char *strchr(const char *aString, s32 aCharacter);

void *sdfChipAllocZeroDirect(s32 aSize);

extern s32 sdfFileCmdAnalyzeFileName(const char *aFileName, char **aRealNamePtr);

extern u16 sdfFileCmdDefaultCreateMode;

extern s32 sdfFileCmdThreadPrio;

extern s32 ChangeThreadPriority(s32 tid, s32 prio);

s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

void sdfCdImageClose(void);

sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                        s32 callbackContext);

s32 sdfFileCmdClearError(sdfFileCmd_t *aHandle);

void sdfFileCmdStartThread(s32 deviceType);

extern s32 (*p2sdSmgBuf)(void);

s32 sdfCdImageGetCurrFileSize(void);

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */ s32 sdfCdImageRead(void *buffer, s32 byteCount);

/* Queue a request on the global list and on its worker's list; wake the worker when its list was empty. */ void sdfFileCmdAllocHandle(sdfFileCmdHandle_t *state);

char *sdfFileGetHostRoot(void);

/* Unlink a cached request under interrupt protection, then release its storage. */
void sdfFileCmdUnlink(void *aHandle)
{
    sdfFileCmdHandle_t *state = aHandle;
    s32 interrupts;
    sdfFileCmdHandle_t *previous;
    sdfFileCmdHandle_t *next;

    interrupts = DIntr();
    previous = state->prev;
    next = state->next;
    if (previous == NULL) {
        D_00551954 = next;
    } else {
        previous->next = next;
    }
    if (next == NULL) {
        D_00551958 = previous;
    } else {
        next->prev = previous;
    }
    sdfFileCmdCount -= 1;
    if (interrupts != 0) {
        EIntr();
    }
    sdfChipFreeDirect(state->filename);
    sdfChipFreeDirect(state);
}

/* Move a completed request off both active lists and into the bounded cache. */
void sdfFileCmdDequeue(void *aCmd)
{
    sdfFileCmdHandle_t *state = aCmd;
    sdfFileDeviceCB_t *worker;
    sdfFileCmdHandle_t *previous;
    sdfFileCmdHandle_t *next;
    s32 interrupts;

    interrupts = DIntr();
    previous = state->prev;
    next = state->next;
    if (previous == NULL) {
        D_00551948 = next;
    } else {
        previous->next = next;
    }
    if (next == NULL) {
        sdfFileCmdFreeList = previous;
    } else {
        next->prev = previous;
    }
    worker = &sdfFileDeviceCB[state->device];
    previous = state->devicePrev;
    next = state->deviceNext;
    if (previous == NULL) {
        worker->first = next;
    } else {
        previous->deviceNext = next;
    }
    if (next == NULL) {
        worker->last = previous;
    } else {
        next->devicePrev = previous;
    }
    state->prev = D_00551958;
    if (D_00551958 == NULL) {
        D_00551954 = state;
    } else {
        D_00551958->next = state;
    }
    D_00551958 = state;
    state->next = NULL;
    sdfFileCmdCount++;
    sdfFileCmdFreeCount--;
    if (interrupts != 0) {
        EIntr();
    }
    if (sdfFileCmdCount >= 9) {
        sdfFileCmdUnlink(D_00551954);
    }
    if (worker->first != NULL) {
        SignalSema(worker->semaId);
    }
}

void sdfFileCmdReleaseHandle(sdfFileCmd_t *aHandle)
{
    s32 fd = aHandle->fd;

    aHandle->fd = -1;
    if (fd >= 0) {
        sceClose(fd);
    }
    sdfFileCmdDequeue(aHandle);
}

/* Store an error/result, release the request, and notify its callback of inactivity. */
void sdfFileCmdFinish(sdfFileCmdHandle_t *state, s32 result) {
    state->errorCode = result;
    state->stat = SDF_FILE_CMD_STAT_ERROR;
    sdfFileCmdReleaseHandle((sdfFileCmd_t *)state);
    if (state->callback != NULL) {
        state->callback(state, 0, 0, 0, state->userData);
    }
}

void sdfFileCmdThread(sdfFileDeviceCB_t *aCB) {
    sdfFileCmdHandle_t *state;
    void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32);
    s32 fileHandle;
    s32 result;
    s32 position;
    s32 transferResult;
    s32 endOffset;
    void *buffer;
    u32 operationIndex;
    for (;;) {
        WaitSema(aCB->semaId);
        state = aCB->first;
        if (state == NULL) {
            continue;
        }
        operationIndex = state->cmd - SDF_FILE_CMD_READOPEN;
        state->cmd = SDF_FILE_CMD_NONE;
        switch (operationIndex) {
        case SDF_FILE_CMD_READOPEN - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_OPEN;
            fileHandle = sceOpen(state->filename, 1, 0);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->stat = SDF_FILE_CMD_STAT_SUSPEND;
            state->fd = fileHandle;
            state->currentOffset = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 2, 0, 0, state->userData);
            }
            continue;
        case SDF_FILE_CMD_WRITEOPEN - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_OPEN;
            fileHandle = sceOpen(state->filename, 0x202, state->cmdParm);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->stat = SDF_FILE_CMD_STAT_SUSPEND;
            state->fd = fileHandle;
            state->currentOffset = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 2, 0, 0, state->userData);
            }
            continue;
        case SDF_FILE_CMD_SEEK - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_SEEK;
            endOffset = sceLseek(state->fd, state->offset, state->cmdParm);
            if (endOffset < 0) {
                sdfFileCmdFinish(state, endOffset);
                continue;
            }
            state->stat = SDF_FILE_CMD_STAT_SUSPEND;
            state->currentOffset = endOffset;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 3, 0, endOffset, state->userData);
            }
            continue;
        case SDF_FILE_CMD_FILESIZE - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_SEEK;
            position = sceLseek(state->fd, 0, 1);
            if (position < 0) {
                sdfFileCmdFinish(state, position);
                continue;
            }
            endOffset = sceLseek(state->fd, 0, 2);
            if (endOffset < 0) {
                sdfFileCmdFinish(state, endOffset);
                continue;
            }
            result = sceLseek(state->fd, position, 0);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->stat = SDF_FILE_CMD_STAT_SUSPEND;
            state->currentOffset = endOffset;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 4, 0, endOffset, state->userData);
            }
            continue;
        case SDF_FILE_CMD_READ - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_READ;
            result = sceRead(state->fd, (s32)state->buffer,
                           state->length);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->stat = SDF_FILE_CMD_STAT_SUSPEND;
            state->currentOffset += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 5, (s32)state->buffer, result,
                                state->userData);
            }
            continue;
        case SDF_FILE_CMD_WRITE - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_WRITE;
            result = sceWrite(state->fd, (const void *)state->buffer, state->length);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->stat = SDF_FILE_CMD_STAT_SUSPEND;
            state->currentOffset += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 6, (s32)state->buffer, result,
                                state->userData);
            }
            continue;
        case SDF_FILE_CMD_CLOSE - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_CLOSE;
            result = sceClose(state->fd);
            state->fd = -1;
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            sdfFileCmdDequeue(state);
            state->stat = SDF_FILE_CMD_STAT_DONE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 7, 0, 0, state->userData);
            }
            continue;
        case SDF_FILE_CMD_READFILE - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_OPEN;
            fileHandle = sceOpen(state->filename, 1, 0);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->fd = fileHandle;
            transferResult = state->length;
            /* A negative count means the whole file; NULL storage is allocated below. */
            if (transferResult < 0) {
                state->stat = SDF_FILE_CMD_STAT_SEEK;
                transferResult = sceLseek(fileHandle, 0, 2);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
                result = sceLseek(state->fd, 0, 0);
                if (result < 0) {
                    sdfFileCmdFinish(state, result);
                    continue;
                }
            }
            if (transferResult != 0) {
                buffer = state->buffer;
                if (buffer == NULL) {
                    buffer = (void *)sdfMemLock((sdfMemHandle_t *)(sdfMemAllocDirect(transferResult)));
                }
                state->buffer = buffer;
                state->stat = SDF_FILE_CMD_STAT_READ;
                transferResult = sceRead(state->fd, (s32)buffer, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            state->stat = SDF_FILE_CMD_STAT_CLOSE;
            sceClose(state->fd);
            state->fd = -1;
            sdfFileCmdDequeue(state);
            state->stat = SDF_FILE_CMD_STAT_DONE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 8, (s32)state->buffer, transferResult,
                         state->userData);
            }
            SignalSema(aCB->semaId);
            continue;
        case SDF_FILE_CMD_WRITEFILE - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_OPEN;
            fileHandle = sceOpen(state->filename, 0x202, state->cmdParm);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->fd = fileHandle;
            transferResult = state->length;
            if (transferResult != 0) {
                state->stat = SDF_FILE_CMD_STAT_WRITE;
                transferResult = sceWrite(fileHandle, (const void *)state->buffer, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            break;
        default:
            continue;
        }
        state->stat = SDF_FILE_CMD_STAT_CLOSE;
        sceClose(state->fd);
        state->fd = -1;
        sdfFileCmdDequeue(state);
        state->stat = SDF_FILE_CMD_STAT_DONE;
        callback = state->callback;
        if (callback != NULL) {
            callback(state, 8, (s32)state->buffer, transferResult,
                            state->userData);
        }
        SignalSema(aCB->semaId);
    }
}

/* Worker slot 3 services disc-backed requests through serialized disc operations. */
void sdfFileCdImageCmdThread(sdfFileDeviceCB_t *aCB) {
    sdfFileCmdHandle_t *state;
    void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32);
    s32 result;
    s32 transferResult;
    void *readBuffer;
    u32 operation;
    for (;;) {
        WaitSema(aCB->semaId);
        state = aCB->first;
        if (state == NULL) {
            continue;
        }
        operation = state->cmd - SDF_FILE_CMD_READOPEN;
        state->cmd = SDF_FILE_CMD_NONE;
        switch (operation) {
        case SDF_FILE_CMD_READOPEN - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_OPEN;
            result = sdfCdImageLoadStart(state->filename);
            if (result < 0) {
                sdfFileCmdFinish(state, 0);
                continue;
            }
            state->stat = SDF_FILE_CMD_STAT_SUSPEND;
            state->currentOffset = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 2, 0, 0, state->userData);
            }
            continue;
        case SDF_FILE_CMD_FILESIZE - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_SUSPEND;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 4, 0, sdfCdImageGetCurrFileSize(),
                                state->userData);
            }
            continue;
        case SDF_FILE_CMD_READ - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_READ;
            result = sdfCdImageRead(state->buffer, state->length);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->stat = SDF_FILE_CMD_STAT_SUSPEND;
            state->currentOffset += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 5, (s32)state->buffer, result,
                                state->userData);
            }
            continue;
        case SDF_FILE_CMD_CLOSE - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_CLOSE;
            sdfCdImageClose();
            sdfFileCmdDequeue(state);
            state->stat = SDF_FILE_CMD_STAT_DONE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 7, 0, 0, state->userData);
            }
            continue;
        case SDF_FILE_CMD_READFILE - SDF_FILE_CMD_READOPEN:
            state->stat = SDF_FILE_CMD_STAT_OPEN;
            result = sdfCdImageLoadStart(state->filename);
            if (result < 0) {
                sdfFileCmdFinish(state, 0);
                continue;
            }
            transferResult = state->length;
            if (transferResult < 0) {
                transferResult = sdfCdImageGetCurrFileSize();
            }
            if (transferResult != 0) {
                readBuffer = state->buffer;
                if (readBuffer == NULL) {
                    readBuffer = (void *)sdfMemLock((sdfMemHandle_t *)(sdfMemAllocDirect(transferResult)));
                }
                state->buffer = readBuffer;
                state->stat = SDF_FILE_CMD_STAT_READ;
                transferResult = sdfCdImageRead(readBuffer, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            state->stat = SDF_FILE_CMD_STAT_CLOSE;
            sdfCdImageClose();
            state->fd = -1;
            sdfFileCmdDequeue(state);
            state->stat = SDF_FILE_CMD_STAT_DONE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 8, (s32)state->buffer, transferResult,
                                state->userData);
            }
            continue;
        case SDF_FILE_CMD_WRITEOPEN - SDF_FILE_CMD_READOPEN:
        case SDF_FILE_CMD_SEEK - SDF_FILE_CMD_READOPEN:
        case SDF_FILE_CMD_WRITE - SDF_FILE_CMD_READOPEN:
        case SDF_FILE_CMD_WRITEFILE - SDF_FILE_CMD_READOPEN:
            sdfFileCmdFinish(state, 0);
            continue;
        default:
            continue;
        }
    }
}

/* Allocate a worker-specific device path; an unsupported worker returns NULL. */
char *sdfFileCmdMakeRealFileName(const char *aSrcName, s32 aDeviceType) {
    s32 length = strlen(aSrcName);
    char *result;
    s32 index;
    s32 character;
    char *base;
    s32 baseLength;
    switch (aDeviceType) {
    case 0:
        result = sdfChipAllocDirect(length + 10);
        memcpy(result, D_00551968, 7);
        index = 0;
        character = aSrcName[index];
        while (character != 0) {
            if (character == '/') character = '\\';
            else if (character >= 'a' && character <= 'z') character -= 0x20;
            result[index + 7] = character;
            index++;
            character = aSrcName[index];
        }
        result[index + 7] = ';';
        result[index + 8] = '1';
        result[index + 9] = 0;
        break;
    case 1:
        if (aSrcName[0] != '/') {
            result = sdfChipAllocDirect(length + 7);
            memcpy(result, D_00551970, 6);
            memcpy(result + 6, aSrcName, length);
            result[length + 6] = 0;
        } else {
            base = sdfFileGetHostRoot();
            baseLength = strlen(base);
            result = sdfChipAllocDirect(length + baseLength);
            memcpy(result, base, baseLength);
            memcpy(result + baseLength, aSrcName + 1, length);
        }
        break;
    case 2:
        result = sdfChipAllocDirect(length + 6);
        memcpy(result, D_00551978, 5);
        memcpy(result + 5, aSrcName, length);
        result[length + 5] = 0;
        break;
    case 3:
        result = sdfChipAllocDirect(length + 1);
        strcpy(result, aSrcName);
        break;
    default:
        return NULL;
    }
    return result;
}

/* DDS engine donor: dds-decomp (redthing1), sdfFileCmdAnalyzeFileName.
 * SMT3 has the /movie/ exception, but not DDS's extra /movie2/ prefix. */
s32 sdfFileCmdAnalyzeFileName(const char *aFileName, char **aRealNamePtr)
{
    s32 device;
    s32 prefixLength;
    const char *suffix;
    const char *name;

    if (strchr(aFileName, ':') != NULL) {
        if (memcmp(aFileName, D_00551968, 7) == 0) {
            suffix = aFileName + 7;
            if (memcmp(suffix, D_00551980, 7) == 0) {
                device = 3;
                prefixLength = 0;
            } else if (memcmp(suffix, "/movie2/", 7) == 0) {
                device = 3;
                prefixLength = 0;
            } else {
                device = 0;
                prefixLength = 7;
            }
        } else if (memcmp(aFileName, D_00551970, 6) == 0) {
            device = 1;
            prefixLength = 6;
        } else if (memcmp(aFileName, D_00551978, 5) == 0) {
            device = 2;
            prefixLength = 5;
        } else if (memcmp(aFileName, D_00551988, 6) == 0) {
            device = 3;
            prefixLength = 6;
        } else {
            return -1;
        }
        name = aFileName + prefixLength;
    } else {
        device = sdfFileCmdDefaultDevice;
        name = aFileName;
    }
    *aRealNamePtr = sdfFileCmdMakeRealFileName(name, device);
    return device;
}

/* Allocate a request state and retain the callback's opaque context word. */
sdfFileCmdHandle_t *sdfFileCmdNewHandle(void *resource, s32 workerIndex, s32 operation,
                        void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32), s32 callbackContext) {
    sdfFileCmdHandle_t *state = (sdfFileCmdHandle_t *)sdfChipAllocZeroDirect(sizeof(sdfFileCmdHandle_t));
    state->filename = resource;
    state->device = workerIndex;
    state->cmd = operation;
    state->stat = 0;
    state->fd = -1;
    state->callback = callback;
    state->userData = callbackContext;
    return state;
}

/* Resolve a path to its worker and enqueue an asynchronous read-only open. */
sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                        s32 callbackContext) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_FILE_CMD_READOPEN, callback, callbackContext);
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Enqueue a write open; zero options select the device's default creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                        s32 callbackContext, s32 options) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_FILE_CMD_WRITEOPEN, callback, callbackContext);
    state->cmdParm = options != 0 ? options : sdfFileCmdDefaultCreateMode;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Queue a seek with its offset and origin; return -1 unless the request is active. */
s32 sdfFileCmdSeek(void *aHandle, s32 aOffset, s32 aWhence) {
    sdfFileCmdHandle_t *state = aHandle;
    if (state->stat != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    state->offset = aOffset;
    state->cmdParm = aWhence;
    state->cmd = SDF_FILE_CMD_SEEK;
    SignalSema(sdfFileDeviceCB[state->device].semaId);
    return 0;
}

/* Query file size without changing the file position; return -1 unless active. */
s32 sdfFileCmdFileSize(void *aHandle) {
    sdfFileCmdHandle_t *state = aHandle;
    if (state->stat != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    state->cmd = SDF_FILE_CMD_FILESIZE;
    SignalSema(sdfFileDeviceCB[state->device].semaId);
    return 0;
}

/* Queue a byte-counted read; return -1 without changing an inactive request. */
s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength) {
    sdfFileCmdHandle_t *state = aHandle;
    if (state->stat != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    state->length = aLength;
    state->cmd = SDF_FILE_CMD_READ;
    state->buffer = aBuffer;
    SignalSema(sdfFileDeviceCB[state->device].semaId);
    return 0;
}

/* Queue a byte-counted write; return -1 without changing an inactive request. */
s32 sdfFileCmdWrite(sdfFileCmdHandle_t *aHandle, const void *aBuffer, s32 aLength) {
    if (aHandle->stat != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    aHandle->length = aLength;
    aHandle->cmd = SDF_FILE_CMD_WRITE;
    aHandle->buffer = (void *)aBuffer;
    SignalSema(sdfFileDeviceCB[aHandle->device].semaId);
    return 0;
}

s32 sdfFileCmdClearError(sdfFileCmd_t *aHandle)
{
    if (aHandle->stat != 9) {
        return -1;
    }
    aHandle->error = 0;
    aHandle->stat = 7;
    return 0;
}

/* Active state and close operation share an ID; retain the original byte assignment. */
s32 sdfFileCmdClose(void *aHandle) {
    sdfFileCmdHandle_t *request = aHandle;
    sdfDevStateCode_t requestState = request->stat;
    if (requestState != SDF_FILE_CMD_STAT_SUSPEND) {
        return -1;
    }
    request->cmd = requestState;
    SignalSema(sdfFileDeviceCB[request->device].semaId);
    return 0;
}

s32 sdfFileCmdFree(sdfFileCmd_t *aHandle)
{
    if (aHandle->stat < SDF_FILE_CMD_STAT_ERROR) {
        if (aHandle->stat >= SDF_FILE_CMD_STAT_SUSPEND) {
            sdfFileCmdUnlink(aHandle);
            return 0;
        }
    }
    return -1;
}

/* Enqueue a one-shot read: negative byte count requests the whole file. */
sdfFileCmdHandle_t *sdfFileCmdReadFile(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32), s32 callbackContext) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_FILE_CMD_READFILE, callback, callbackContext);
    state->length = byteCount;
    state->buffer = (void *)buffer;
    state->offset = 0;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Enqueue a one-shot write using the supplied buffer, byte count and creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteFile(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                        s32 callbackContext, s32 options) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_FILE_CMD_WRITEFILE, callback, callbackContext);
    state->length = byteCount;
    state->buffer = (void *)buffer;
    state->cmdParm = options != 0 ? options : sdfFileCmdDefaultCreateMode;
    state->offset = 0;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Update non-negative worker IDs only; skip an unchanged numeric priority. */
void sdfFileCmdThreadSetPrio(s32 prio) {
    sdfFileDeviceCB_t *worker;
    u32 index;
    if (sdfFileCmdThreadPrio == prio) {
        return;
    }
    sdfFileCmdThreadPrio = prio;
    worker = sdfFileDeviceCB;
    index = 0;
    do {
        s32 threadId = worker->thId;
        worker++;
        if (threadId >= 0) {
            ChangeThreadPriority(threadId, prio);
        }
        index++;
    } while (index < 4);
}

void sdfFileCmdThreadSetLoPrio(void)
{
    sdfFileCmdThreadPrioCheckCounter = 3;
    sdfFileCmdThreadSetPrio(0x78);
}

void sdfFileCmdThreadSetHiPrio(void)
{
    sdfFileCmdThreadSetPrio(0x48);
}

void sdfFileCmdThreadCheckPrio(void)
{
    if (sdfFileCmdThreadPrioCheckCounter != 0) {
        sdfFileCmdThreadPrioCheckCounter--;
        if (sdfFileCmdThreadPrioCheckCounter == 0) {
            sdfFileCmdThreadSetHiPrio();
        }
    }
}

/* Start worker thread `index` if it isn't running; slot 3 runs the alternate entry point. */
void sdfFileCmdStartThread(s32 deviceType) {
    sdfFileDeviceCB_t *worker = &sdfFileDeviceCB[deviceType];
    void (*entry)();
    s32 thread;
    thread = worker->thId;
    if (thread < 0) {
        worker->semaId = sdfCreateSema(0, 0xFF, 0);
        entry = sdfFileCdImageCmdThread;
        if (deviceType != 3) {
            entry = sdfFileCmdThread;
        }
        sdfFileCmdThreadPrio = 0x48;
        thread = sdfCreateThreadWithStack(entry, 0x4000, 0x48);
        worker->thId = thread;
        _StartThread(thread, worker);
    }
}

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdCount);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdCacheHead);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdCacheTail);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdFreeCount);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdDefaultDevice);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdThreadPrioCheckCounter);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdThreadPrio);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdDefaultCreateMode);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdCdromPrefix);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdHostPrefix);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdPfsPrefix);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdMoviePath);

INCLUDE_SDATA(const s32, "sdf/sdfFileIO", sdfFileCmdCdimgPrefix);
