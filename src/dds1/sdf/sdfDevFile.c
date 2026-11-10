#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

void *sdfDevLoadFile(const char *aFileName, u32 *aOutData, u32 *aOutSize, s32 aAlign);

typedef struct
{
    u8 unk00[0x10];
    s32 resolveOffset;
    s32 resolveCount;
    u8 unk18[8];
} sdfBinaryHeader_t;

void sdfAddressResolver(void *aDataBody, void *aOffset, void *aAddrInfoPtr, s32 aAddrInfoLeng);

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
    void (*callback)(struct sdfFileCmdHandle_t *, s32, saddr_t, s32, s32);
    s32 userData;
} sdfFileCmdHandle_t;

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

typedef struct sdfTextureOffsetListEntry_t {
    s32 textureHeaderOffset;
} sdfTextureOffsetListEntry_t;

typedef struct sdfTextureOffsetListHeader_t {
    u8 unknown00[0x10];
    s32 textureCount;
} sdfTextureOffsetListHeader_t;

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

u32 sdfMemLock(sdfMemHandle_t *aHandle);

#include "sdk_libc.h"


void sdfMemUnlock(sdfMemHandle_t *aHandle);

void sdfTexListAdd(sdfItemArray_t *aList, sdfTexHandle_t *aTexHandle);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern sdfItemArray_t *sdfTexListNew(u32 aInitCapa);

sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

extern s32 (*p2sdSmgBuf)(void);

void sdfFileClose(void *aHandle);

u32 sdfFileLength(void *aHandle);

/* Open a path using the shared reply callback and wait for its initial completion. */ sdfFileCmdHandle_t *sdfFileOpen(const char *path);

s32 sdfFileRead(void *aHandle, void *aPtr, s32 aLeng);

/* Read a named file through the dev RPC into a freshly allocated block; returns the block's handle.
 * outData receives the block address, outSize the file size; without outData the block is released. */
void *sdfDevLoadFile(const char *name, u32 *outData, u32 *outSize, s32 extra) {
    sdfFileCmdHandle_t *state = sdfFileOpen(name);
    s32 size = sdfFileLength(state);
    sdfMemHandle_t *handle = sdfMemAllocDirect(size + extra);
    u32 address = sdfMemLock(handle);
    sdfFileRead(state, EE_ADDR_AS(void *, address), size);
    sdfFileClose(state);
    if (outData != NULL) {
        *outData = address;
    } else {
        sdfMemUnlock(handle);
    }
    if (outSize != NULL) {
        *outSize = size;
    }
    return handle;
}

void *sdfDevLoadFileEx(const char *aFileName, u32 *aOutData, u32 *aOutSize)
{
    return sdfDevLoadFile(aFileName, aOutData, aOutSize, 0);
}

sdfTexHandle_t *sdfDevLoadTex(const char *aFilename) {
    u32 dataAddress;
    sdfMemHandle_t *allocation;
    sdfTexHandle_t *texture;

    allocation = sdfDevLoadFileEx(aFilename, &dataAddress, 0);
    texture = sdfTexSet(EE_ADDR_AS(void *, dataAddress));
    sdfMemFreeDirect(allocation);
    return texture;
}

sdfItemArray_t *sdfTexSetTB(const sdfTextureOffsetListHeader_t *aData) {
    s32 i = 0;
    s32 count = aData->textureCount;
    sdfItemArray_t *handle = sdfTexListNew(count);
    const sdfTextureOffsetListEntry_t *entry;
    if (count != i) {
        entry = (const sdfTextureOffsetListEntry_t *)((const u8 *)aData + sizeof(*aData));
        do {
            i++;
            sdfTexListAdd(handle, sdfTexSet((TMX_HEADER_t *)(
                (void *)((const u8 *)aData + entry->textureHeaderOffset))));
            entry++;
        } while (i != count);
    }
    return handle;
}

sdfItemArray_t *sdfDevLoadTexList(const char *aFilename) {
    u32 dataAddress;
    sdfMemHandle_t *allocation;
    sdfItemArray_t *texture;

    allocation = sdfDevLoadFileEx(aFilename, &dataAddress, 0);
    texture = sdfTexSetTB(EE_ADDR_AS(void *, dataAddress));
    sdfMemFreeDirect(allocation);
    return texture;
}

void *sdfFixupModelBinary(sdfBinaryHeader_t *aData)
{
    void *body = aData + 1;

    sdfAddressResolver(body, body, (u8 *)body + aData->resolveOffset, aData->resolveCount);
    return body;
}

void *sdfDevLoadModel(const char *aFilename, void **aOutBody) {
    u32 dataAddress;
    sdfMemHandle_t *allocation;

    allocation = sdfDevLoadFileEx(aFilename, &dataAddress, 0);
    *aOutBody = sdfFixupModelBinary(EE_ADDR_AS(sdfBinaryHeader_t *, dataAddress));
    return allocation;
}

void *sdfFixupMotionBinary(sdfBinaryHeader_t *aData)
{
    void *body = aData + 1;

    sdfAddressResolver(body, body, (u8 *)body + aData->resolveOffset, aData->resolveCount);
    return body;
}

void *sdfDevLoadMotion(const char *aFilename, void **aOutBody) {
    u32 dataAddress;
    sdfMemHandle_t *allocation;

    allocation = sdfDevLoadFileEx(aFilename, &dataAddress, 0);
    *aOutBody = sdfFixupMotionBinary(EE_ADDR_AS(sdfBinaryHeader_t *, dataAddress));
    return allocation;
}


