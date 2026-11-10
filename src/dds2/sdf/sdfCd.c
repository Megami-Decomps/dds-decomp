#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"


typedef struct
{
    u32 nameOffset_;
    u32 offset_;
    s32 size_;
} sdfCdImageDirEntry_t;

s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

s32 sceCdInit(s32 aMode);

s32 sceCdMmode(s32 aMode);

s32 sceCdStStop(void);

extern sdfCdImageDirEntry_t *sdfCdImageCurrFile;

extern s32 sdfCdImageFd;

extern s32 sdfCdTrayMode;

sdfCdImageDirEntry_t *sdfCdImageSearchFile(const char *fname, s32 *lsn, u8 *modePtr);

extern s32 WaitSema(s32 aSemaId);

extern s32 SignalSema(s32 aSemaId);

extern void *sdfCdImageDirData;


void FatalError(const char *aMessage, ...) __attribute__((noreturn));

s32 sceClose(s32 a);

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

extern void *memcpy(void *, const void *, size_t);

extern void sdfDelayThread(s32 delay);

void sdfCdWaitTrayClose(void);

extern u8 sdfCdImageMode;

extern s32 sdfCdCurrLsn;

extern s32 sceCdStStart(s32 arg, u8 *options);

extern s32 sceCdStatus(void);

extern s32 sdfCdImageCurrRemain;

extern s32 sdfCdImageCurrActive;

extern u8 *sdfCdImageOffset;

extern u8 sdfCdSectorBuf[0x800];

#include "sdk_libkernl.h"

extern s32 sceLseek(s32 fileHandle, s32 offset, s32 origin);

extern void *malloc(size_t size);

extern s32 sceRead(s32 fileHandle, saddr_t buffer, s32 byteCount);

extern const char *dds3ImgPath;

typedef struct sceCdlFILE sceCdlFILE;
#include "sdk_libcdvd.h"

extern u32 sdfCdImageFileName;

extern u32 dds3ImgLsn;

extern s32 sceSifAllocIopHeap(s32);

extern void sceCdStInit(s32, s32, s32);

void sdfCdImageClose(void);

extern s32 (*p2sdSmgBuf)(void);

void *sdfCdImageGetDirData(void)
{
    return sdfCdImageDirData;
}

#include "sdk_libc.h"

extern s32 sceCdDiskReady(s32 mode);
extern s32 sceCdGetDiskType(void);
extern char sdfCdMsgWaitTrayClose[]; /* "wait tray close.\n" */
extern char sdfCdMsgTrayClose[]; /* "tray close.\n" */
extern char sdfCdMsgReadRestart[]; /* "CD read restart.\n" */

void sdfCdWaitTrayClose(void)
{
    u8 file[0x30];
    s32 status;

    WaitSema(sdfCdImageFd);
    for (;;) {
        sdfDelayThread(100);
        sceCdDiskReady(0);
        status = sceCdGetDiskType();
        if (sdfCdTrayMode == 2) {
            if (status != 20) {
                continue;
            }
        } else if (status != 18) {
            continue;
        }
        if (sceCdSearchFile((sceCdlFILE *)file, (const char *)dds3ImgPath) != 0) {
            break;
        }
    }
    SignalSema(sdfCdImageFd);
}

void sdfCdStStart(s32 lsn) {
    u8 options[4];
    s32 ready;
    options[0] = 0;
    if (sdfCdImageMode == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(sdfCdImageFd);
        ready = sceCdStStart(lsn, options);
        SignalSema(sdfCdImageFd);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfCdWaitTrayClose();
        } else {
            sdfDelayThread(100);
        }
    }
    sdfCdCurrLsn = lsn;
}


extern s32 sdfDiscRequestPending;
extern s32 sdfDiscRequestSemaphore;

void sdfDevSignalPendingSemaphore(void)
{
    if (sdfDiscRequestPending != 0) {
        SignalSema(sdfDiscRequestSemaphore);
        sdfDiscRequestPending = 0;
    }
}

void sdfDevSeekDiscWithRequestGate(s32 lsn)
{
    u8 options[4];
    s32 ready;

    options[0] = 0;
    if (sdfCdImageMode == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(sdfCdImageFd);
        sdfDiscRequestPending = 1;
        WaitSema(sdfDiscRequestSemaphore);
        ready = sceCdStStart(lsn, options);
        SignalSema(sdfCdImageFd);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfCdWaitTrayClose();
        } else {
            sdfDelayThread(100);
        }
    }
    sdfCdCurrLsn = lsn;
}

void sdfCdStRead(s32 sectors, void *buffer)
{
    s32 status;
    s32 result;

    for (;;) {
        WaitSema(sdfCdImageFd);
        result = sceCdStRead(sectors, (u32 *)buffer, 1, (u32 *)&status);
        SignalSema(sdfCdImageFd);
        if (status == 0 && result == sectors) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfCdWaitTrayClose();
            sdfCdStStart(sdfCdCurrLsn);
        } else {
            WaitSema(sdfCdImageFd);
            sceCdStStop();
            SignalSema(sdfCdImageFd);
            sdfCdStStart(sdfCdCurrLsn);
        }
    }
    sdfCdCurrLsn += sectors;
}

extern char *strcpy(char *destination, const char *source);
extern char *strchr(const char *text, s32 character);
extern sdfCdImageDirEntry_t sdfCdImageSearchFileDirect_ent;

sdfCdImageDirEntry_t *sdfCdImageSearchFileDirect(const char *filename, s32 *lsn, u8 *mode)
{
    u32 file[9];
    char path[256];
    char *name;
    char *cursor;
    s32 character;
    s32 status;
    s32 fd;

    strcpy(path, filename);
    name = strchr(path, ':') + 1;
    cursor = name;
    character = *cursor;
    while (character != 0) {
        if (character == '/') {
            *cursor = '\\';
        } else if (character >= 'a' && character <= 'z') {
            *cursor = character - 0x20;
        }
        cursor++;
        character = *cursor;
    }
    if (cursor[-2] != ';') {
        cursor[0] = ';';
        cursor[1] = '1';
        cursor[2] = 0;
    }
    for (;;) {
        WaitSema(sdfCdImageFd);
        if (sceCdSearchFile((sceCdlFILE *)file, (const char *)name) != 0) {
            fd = sceOpen(path, 1);
            if (fd < 0) {
                SignalSema(sdfCdImageFd);
                continue;
            }
            sdfCdImageSearchFileDirect_ent.size_ = sceLseek(fd, 0, 2);
            sceClose(fd);
            SignalSema(sdfCdImageFd);
            break;
        }
        status = sceCdStatus();
        SignalSema(sdfCdImageFd);
        if (status == 1) {
            sdfCdWaitTrayClose();
        } else {
            sdfDelayThread(100);
        }
    }
    *lsn = file[0];
    if (mode != NULL) {
        *mode = 1;
    }
    return &sdfCdImageSearchFileDirect_ent;
}

extern char sdfCdDirectPrefix[]; /* "cdrom" */
extern s32 memcmp(const void *left, const void *right, size_t size);
extern s32 strcmp(const char *left, const char *right);

sdfCdImageDirEntry_t *sdfCdImageSearchFile(const char *path, s32 *lsn, u8 *mode)
{
    char name[32];
    const char *segment;
    const char *cursor;
    sdfCdImageDirEntry_t *entries;
    sdfCdImageDirEntry_t *node;
    u8 *base;
    s32 character;
    u32 length;
    s32 low;
    s32 high;
    s32 mid;
    s32 order;

    if (memcmp(path, sdfCdDirectPrefix, 5) == 0) {
        return sdfCdImageSearchFileDirect(path, lsn, mode);
    }
    base = sdfCdImageDirData;
    node = (sdfCdImageDirEntry_t *)base;
    segment = path;
    do {
        character = *segment;
        if (character == '/') {
            segment++;
        }
        cursor = segment;
        while (*cursor != '/' && *cursor != 0) {
            cursor++;
        }
        length = cursor - segment;
        if (length >= 32) {
            FatalError("filename too long : %s", path);
        }
        memcpy(name, segment, length);
        name[length] = 0;
        entries = (sdfCdImageDirEntry_t *)(base + node->offset_);
        low = 0;
        high = -node->size_;
        for (;;) {
            if (low >= high) {
                return NULL;
            }
            mid = (low + high) >> 1;
            node = &entries[mid];
            order = strcmp(name, (char *)(base + node->nameOffset_));
            if (order == 0) {
                break;
            }
            if (order < 0) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
        segment = cursor;
    } while (*cursor != 0);
    *lsn = dds3ImgLsn + node->offset_;
    if (mode != NULL) {
        *mode = 0;
    }
    return node;
}

s32 sdfCdImageLoadStart(const char *filename)
{
    s32 result;
    s32 lsn[4];
    sdfCdImageDirEntry_t *entry;

    entry = sdfCdImageSearchFile(filename, lsn, &sdfCdImageMode);
    result = -1;
    if (entry != NULL) {
        sdfCdImageCurrRemain = entry->size_;
        sdfCdImageCurrFile = entry;
        sdfCdImageCurrActive = 0;
        if (filename[1] == 'v' || filename[1] == 'V') {
            sdfDevSeekDiscWithRequestGate(lsn[0]);
        } else {
            sdfCdStStart(lsn[0]);
        }
        result = entry->size_;
    }
    return result;
}

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */
s32 sdfCdImageRead(void *buff, s32 length) {
    s32 available = sdfCdImageCurrRemain;
    u8 *destination = buff;
    s32 bytes;
    s32 transferBytes;
    s32 t;
    if (available <= 0) {
        return 0;
    }
    if (length > available) {
        length = available;
    }
    if (length == 0) {
        return 0;
    }
    bytes = length;
    transferBytes = sdfCdImageCurrActive;
    if (transferBytes > 0) {
        if (transferBytes > length) {
            transferBytes = length;
        }
        memcpy(destination, sdfCdImageOffset, transferBytes);
        destination += transferBytes;
        sdfCdImageOffset += transferBytes;
        sdfCdImageCurrActive -= transferBytes;
        bytes -= transferBytes;
    }
    if (bytes > 0) {
        transferBytes = bytes;
        if ((EE_ADDR(destination) & 15) == 0) {
            t = transferBytes >> 11;
            t = transferBytes - t * 0x800;
            transferBytes -= t;
            if (transferBytes > 0) {
                sdfCdStRead(transferBytes >> 11, destination);
            }
            if (t > 0) {
                sdfCdStRead(1, sdfCdSectorBuf);
                memcpy(destination + transferBytes, sdfCdSectorBuf, t);
                sdfCdImageOffset = sdfCdSectorBuf + t;
                sdfCdImageCurrActive = 0x800 - t;
            }
        } else {
            while (transferBytes >= 0x800) {
                sdfCdStRead(1, sdfCdSectorBuf);
                memcpy(destination, sdfCdSectorBuf, 0x800);
                transferBytes -= 0x800;
                destination += 0x800;
            }
            if (transferBytes > 0) {
                sdfCdStRead(1, sdfCdSectorBuf);
                memcpy(destination, sdfCdSectorBuf, transferBytes);
                sdfCdImageOffset = sdfCdSectorBuf + transferBytes;
                sdfCdImageCurrActive = 0x800 - transferBytes;
            }
        }
    }
    sdfCdImageCurrRemain = available - length;
    if (sdfCdImageCurrRemain == 0) {
        sdfCdImageClose();
    }
    return length;
}

void sdfCdImageClose(void)
{
    if (sdfCdImageCurrFile != 0) {
        WaitSema(sdfCdImageFd);
        sceCdStStop();
        SignalSema(sdfCdImageFd);
        sdfCdImageCurrFile = NULL;
    }
}

s32 sdfCdImageCheckExist(const char *fname)
{
    s32 lsn;

    return sdfCdImageSearchFile(fname, &lsn, NULL) != 0;
}

s32 sdfCdImageGetFileSize(const char *fname)
{
    s32 lsn;
    sdfCdImageDirEntry_t *entry = sdfCdImageSearchFile(fname, &lsn, NULL);

    if (entry == NULL) {
        return -1;
    }
    return entry->size_;
}

s32 sdfCdImageGetCurrFileSize(void)
{
    sdfCdImageDirEntry_t *file = sdfCdImageCurrFile;

    if (file == NULL) {
        FatalError("file didn't open.");
    }
    return file->size_;
}

/* Read a whole named file into the loader's allocated EE buffer, then close it. */
void sdfCdImageLoadDir(const char *dirFile) {
    s32 fileHandle = sceOpen(dirFile, 1);
    s32 fileBytes;
    void *buffer;
    if (fileHandle < 0) {
        FatalError("%s couldn't open.", dirFile);
    }
    fileBytes = sceLseek(fileHandle, 0, 2);
    sceLseek(fileHandle, 0, 0);
    buffer = malloc(fileBytes);
    sdfCdImageDirData = buffer;
    sceRead(fileHandle, EE_ADDR_S(buffer), fileBytes);
    sceClose(fileHandle);
}

/* Record the disc search path, preload a separate file, and initialize an aligned IOP buffer. */
void sdfCdImageInit(const char *imageFile, const char *dirFile) {
    u32 discFileRecord[12];
    s32 iopHeapAddress;
    dds3ImgPath = imageFile;
    sdfCdImageLoadDir(dirFile);
    if (sceCdSearchFile((sceCdlFILE *)discFileRecord, (const char *)imageFile) == 0) {
        FatalError("%s couldn't open.", imageFile);
    }
    dds3ImgLsn = discFileRecord[0];
    iopHeapAddress = sceSifAllocIopHeap(0x28010);
    sdfCdImageFileName = iopHeapAddress;
    sceCdStInit(0x50, 5, (iopHeapAddress + 15) & -16);
}

void sdfCdInit(void)
{
    sdfCdImageFd = sdfCreateSema(1, 0xFF, 0);
    sdfDiscRequestSemaphore = sdfCreateSema(0, 0xFF, 0);
    sdfDiscRequestPending = 0;
    sceCdInit(0);
    sceCdMmode(sdfCdTrayMode);
}
INCLUDE_SDATA(const s32, "sdf/sdfCd", sdfDiscRequestPending);

INCLUDE_SDATA(const s32, "sdf/sdfCd", sdfCdDirectPrefix);

INCLUDE_SDATA(const s32, "sdf/sdfCd", sdfCdTrayMode);

