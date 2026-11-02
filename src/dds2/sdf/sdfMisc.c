#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

void sdfChipFreeDirect(void *aPtr);

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

void *sdfChipAllocDirect(s32 aSize);

u32 sdfMemLock(sdfMemHandle_t *aHandle);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern s32 (*p2sdSmgBuf)(void);

/* Allocate element storage with a fixed element stride and growth increment. */
sdfItemArray_t *sdfItemArrayNew(s32 aInitCapa, s32 aElemSize, s32 aGrowStep) {
    sdfItemArray_t *request = sdfChipAllocDirect(sizeof(*request));
    request->growStep = aGrowStep;
    request->count = 0;
    request->capacity = aInitCapa;
    request->elemSize = aElemSize;
    if (aInitCapa != 0) {
        request->memHandle = sdfMemAllocDirect(aElemSize * aInitCapa);
        request->items = (void *)sdfMemLock(request->memHandle);
    } else {
        request->memHandle = 0;
        request->items = 0;
    }
    return request;
}

void sdfItemArrayFree(sdfItemArray_t *aArray)
{
    sdfMemFreeDirect(aArray->memHandle);
    sdfChipFreeDirect(aArray);
}

void sdfMemUnlock(sdfMemHandle_t *aHandle);

void sdfMemReallocRecord(sdfMemHandle_t *aHandle, s32 aSize);

#define func_002BFF98 sdfDevResizeBufferedRequest

void func_002BFF98(sdfItemArray_t *aArray, s32 aCapacity);

void sdfItemArrayGrow(sdfItemArray_t *aArray)
{
    if (aArray->memHandle == NULL) {
        func_002BFF98(aArray, aArray->growStep);
        return;
    }
    sdfMemUnlock(aArray->memHandle);
    aArray->capacity = aArray->capacity + aArray->growStep;
    sdfMemReallocRecord(aArray->memHandle, aArray->capacity * aArray->elemSize);
    aArray->items = (void *)sdfMemLock(aArray->memHandle);
}

void func_002BFF98(sdfItemArray_t *aArray, s32 aCapacity)
{
    if (aArray->memHandle == NULL) {
        if (aCapacity > 0) {
            aArray->capacity = aCapacity;
            aArray->memHandle = sdfMemAllocDirect(aArray->elemSize * aCapacity);
            aArray->items = (void *)sdfMemLock(aArray->memHandle);
        }
    } else if (aCapacity <= 0) {
        sdfMemFreeDirect(aArray->memHandle);
        aArray->memHandle = NULL;
        aArray->count = 0;
        aArray->capacity = 0;
        aArray->items = NULL;
    } else {
        sdfMemUnlock(aArray->memHandle);
        aArray->capacity = aCapacity;
        sdfMemReallocRecord(aArray->memHandle, aArray->elemSize * aCapacity);
        aArray->items = (void *)sdfMemLock(aArray->memHandle);
        if (aCapacity < aArray->count) {
            aArray->count = aCapacity;
        }
    }
}
