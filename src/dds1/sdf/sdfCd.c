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
#include "sdf_heap_api.h"
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

/* Declared before first use (were implicit int). */
extern void sceCdInit(s32);

#define SDF_DEV_FILE_OPEN_READ 1

#define SDF_DEV_SEEK_START 0

#define SDF_DEV_SEEK_END 2

#define SDF_DEV_DISC_FILE_WORDS 12

#define SDF_DEV_DISC_SECTOR_BYTES 0x800

#define SDF_DEV_DISC_SECTOR_SHIFT 11

#define SDF_DEV_DISC_ALIGNMENT_MASK 15

#define SDF_DEV_IOP_BUFFER_BYTES 0x28010

#define SDF_DEV_IOP_ALIGNMENT_BIAS 15

#define SDF_DEV_IOP_ALIGNMENT_MASK -16

extern SdfDiscFileRecord *sdfCdImageCurrFile;

extern u32 sdfCdImageFd;

extern s32 sdfDiscRequestPending;

extern u32 sdfDiscRequestSemaphore;

extern u32 D_003BDA44;

extern void FatalError(const char *format, ...) __attribute__((noreturn));

extern void sceCdMmode(u32 arg0);

extern u32 sdfCdTrayMode;

extern void sdfDelayThread(s32);

void sdfDevWaitForDisc(void);

extern s32 dds3ImgPath;

extern void sceCdDiskReady(s32);

extern s32 sceCdGetDiskType(void);

extern SdfDiscReadMode sdfDiscReadMode;

extern s32 sdfDiscReadPosition;

extern s32 sceCdStStart(s32 arg, u8 *options);

extern s32 sceCdStRead(s32 size, s32 buffer, s32 arg2, s32 *status);

extern s32 sceCdStStop(void);

extern s32 D_003BDA4C;

extern s32 sdfCdImageCurrActive;

extern u8 *sdfCdImageOffset;

extern u8 sdfDevReadSectorBuffer[SDF_DEV_DISC_SECTOR_BYTES];

extern void sdfCdImageClose(void);

u32 sdfCdImageGetDirData(void) {
    return D_003BDA44;
}

void sdfDevWaitForDisc(void) {
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
        if (sceCdSearchFile(file, dds3ImgPath) != 0) {
            break;
        }
    }
    SignalSema(sdfCdImageFd);
}

void sdfCdStStart(s32 request) {
    u8 options[4];
    s32 ready;

    options[0] = 0;
    if (sdfDiscReadMode == SDF_DISC_READ_MODE_CDROM_STREAM) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(sdfCdImageFd);
        ready = sceCdStStart(request, options);
        SignalSema(sdfCdImageFd);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
        } else {
            sdfDelayThread(100);
        }
    }
    sdfDiscReadPosition = request;
}

void sdfDevSignalPendingSemaphore(void) {
    if (sdfDiscRequestPending != 0) {
        SignalSema(sdfDiscRequestSemaphore);
        sdfDiscRequestPending = 0;
    }
}

void sdfDevSeekDiscWithRequestGate(s32 request) {
    u8 options[4];
    s32 ready;

    options[0] = 0;
    if (sdfDiscReadMode == SDF_DISC_READ_MODE_CDROM_STREAM) {
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
        ready = sceCdStStart(request, options);
        SignalSema(sdfCdImageFd);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
        } else {
            sdfDelayThread(100);
        }
    }
    sdfDiscReadPosition = request;
}

void sdfDevReadDiscUntilComplete(s32 size, void *buffer) {
    s32 status;
    s32 result;

    for (;;) {
        WaitSema(sdfCdImageFd);
        result = sceCdStRead(size, (s32)buffer, 1, &status);
        SignalSema(sdfCdImageFd);
        if (status == 0 && result == size) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
            sdfCdStStart(sdfDiscReadPosition);
        } else {
            WaitSema(sdfCdImageFd);
            sceCdStStop();
            SignalSema(sdfCdImageFd);
            sdfCdStStart(sdfDiscReadPosition);
        }
    }
    sdfDiscReadPosition += size;
}

extern SdfDiscFileRecord D_003F9FC0;

/* Look up a "cdrom" path through the CD file system: convert it to an ISO
 * name ("\\" separators, upper case, ";1" version), retry the search until
 * the disc answers, and record the file's size in the shared record. */
SdfDiscFileRecord *sdfDevFindDiscFileRecordByCdromPath(
    const char *path, u32 *readLocationOut, SdfDiscReadMode *readModeOut) {
    u32 file[9];
    char normalizedPath[256];
    char *name;
    char *cursor;
    s32 character;
    s32 status;
    s32 fd;

    strcpy(normalizedPath, path);
    name = strchr(normalizedPath, ':') + 1;
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
        if (sceCdSearchFile(file, (s32)name) != 0) {
            fd = sceOpen(normalizedPath, 1);
            if (fd < 0) {
                SignalSema(sdfCdImageFd);
                continue;
            }
            D_003F9FC0.sizeOrNegativeChildCount = sceLseek(fd, 0, 2);
            sceClose(fd);
            SignalSema(sdfCdImageFd);
            break;
        }
        status = sceCdStatus();
        SignalSema(sdfCdImageFd);
        if (status == 1) {
            sdfDevWaitForDisc();
        } else {
            sdfDelayThread(100);
        }
    }
    *readLocationOut = file[0];
    if (readModeOut != 0) {
        *readModeOut = SDF_DISC_READ_MODE_CDROM_STREAM;
    }
    return &D_003F9FC0;
}

extern u32 D_003BDA40;

extern char D_003BD3E0[]; /* cdrom */

extern char D_003B4548[]; /* filename too long : %s */

/* Walk a '/'-separated path through the preloaded disc index: each directory
 * holds its children sorted by name, with a negative size whose magnitude is
 * the child count, so every segment is a binary search. */
SdfDiscFileRecord *sdfCdImageSearchFile(
    const char *path, u32 *readLocationOut, SdfDiscReadMode *readModeOut) {
    char name[32];
    const char *segment;
    const char *cursor;
    SdfDiscFileRecord *entries;
    SdfDiscFileRecord *node;
    u32 base;
    s32 character;
    u32 length;
    s32 low;
    s32 high;
    s32 mid;
    s32 order;

    if (memcmp(path, D_003BD3E0, 5) == 0) {
        return sdfDevFindDiscFileRecordByCdromPath(path, readLocationOut, readModeOut);
    }
    base = D_003BDA44;
    node = (SdfDiscFileRecord *)base;
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
            FatalError(D_003B4548, path);
        }
        memcpy(name, segment, length);
        name[length] = 0;
        entries = (SdfDiscFileRecord *)(base + node->dataOffset);
        low = 0;
        high = -node->sizeOrNegativeChildCount;
        for (;;) {
            if (low >= high) {
                return NULL;
            }
            mid = (low + high) >> 1;
            node = &entries[mid];
            order = strcmp(name, (char *)(base + node->nameOffset));
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
    *readLocationOut = D_003BDA40 + node->dataOffset;
    if (readModeOut != 0) {
        *readModeOut = SDF_DISC_READ_MODE_INDEXED_IMAGE;
    }
    return node;
}

s32 sdfCdImageLoadStart(const char *name) {
    s32 result;
    u32 readLocation[4];
    SdfDiscFileRecord *file;

    file = sdfCdImageSearchFile(name, readLocation, &sdfDiscReadMode);
    result = -1;
    if (file != 0) {
        D_003BDA4C = file->sizeOrNegativeChildCount;
        sdfCdImageCurrFile = file;
        sdfCdImageCurrActive = 0;
        if (name[1] == 0x76 || name[1] == 0x56) {
            sdfDevSeekDiscWithRequestGate(readLocation[0]);
        } else {
            sdfCdStStart(readLocation[0]);
        }
        result = file->sizeOrNegativeChildCount;
    }
    return result;
}

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */
s32 sdfCdImageRead(void *buffer, s32 byteCount) {
    s32 available = D_003BDA4C;
    u8 *destination = buffer;
    s32 bytes;
    s32 transferBytes;
    s32 t;

    if (available <= 0) {
        return 0;
    }
    if (byteCount > available) {
        byteCount = available;
    }
    if (byteCount == 0) {
        return 0;
    }

    bytes = byteCount;
    transferBytes = sdfCdImageCurrActive;
    if (transferBytes > 0) {
        if (transferBytes > byteCount) {
            transferBytes = byteCount;
        }
        memcpy(destination, sdfCdImageOffset, transferBytes);
        destination += transferBytes;
        sdfCdImageOffset += transferBytes;
        sdfCdImageCurrActive -= transferBytes;
        bytes -= transferBytes;
    }
    if (bytes > 0) {
        transferBytes = bytes;
        if (((u32)destination & SDF_DEV_DISC_ALIGNMENT_MASK) == 0) {
            t = transferBytes >> SDF_DEV_DISC_SECTOR_SHIFT;
            t = transferBytes - t * SDF_DEV_DISC_SECTOR_BYTES;
            transferBytes -= t;
            if (transferBytes > 0) {
                sdfDevReadDiscUntilComplete(transferBytes >> SDF_DEV_DISC_SECTOR_SHIFT, destination);
            }
            if (t > 0) {
                sdfDevReadDiscUntilComplete(1, sdfDevReadSectorBuffer);
                memcpy(destination + transferBytes, sdfDevReadSectorBuffer, t);
                sdfCdImageOffset = sdfDevReadSectorBuffer + t;
                sdfCdImageCurrActive = SDF_DEV_DISC_SECTOR_BYTES - t;
            }
        } else {
            while (transferBytes >= SDF_DEV_DISC_SECTOR_BYTES) {
                sdfDevReadDiscUntilComplete(1, sdfDevReadSectorBuffer);
                memcpy(destination, sdfDevReadSectorBuffer, SDF_DEV_DISC_SECTOR_BYTES);
                transferBytes -= SDF_DEV_DISC_SECTOR_BYTES;
                destination += SDF_DEV_DISC_SECTOR_BYTES;
            }
            if (transferBytes > 0) {
                sdfDevReadDiscUntilComplete(1, sdfDevReadSectorBuffer);
                memcpy(destination, sdfDevReadSectorBuffer, transferBytes);
                sdfCdImageOffset = sdfDevReadSectorBuffer + transferBytes;
                sdfCdImageCurrActive = SDF_DEV_DISC_SECTOR_BYTES - transferBytes;
            }
        }
    }
    D_003BDA4C = available - byteCount;
    if (D_003BDA4C == 0) {
        sdfCdImageClose();
    }
    return byteCount;
}

void sdfCdImageClose(void) {
    if (sdfCdImageCurrFile != 0) {
        WaitSema(sdfCdImageFd);
        sceCdStStop();
        SignalSema(sdfCdImageFd);
        sdfCdImageCurrFile = 0;
    }
}

s32 sdfCdImageCheckExist(const char *path) {
    SdfDiscFileRecord *record;
    u32 readLocation[4];

    record = sdfCdImageSearchFile(path, readLocation, 0);
    return record != 0;
}

s32 sdfCdImageGetFileSize(const char *path) {
    u32 readLocation[4];
    SdfDiscFileRecord *record;

    record = sdfCdImageSearchFile(path, readLocation, 0);
    if (record != 0) {
        return record->sizeOrNegativeChildCount;
    }
    return -1;
}

s32 sdfCdImageGetCurrFileSize(void) {
    if (sdfCdImageCurrFile == 0) {
        FatalError("file didn't open.");
    }
    return sdfCdImageCurrFile->sizeOrNegativeChildCount;
}

extern void *malloc(u32 size);

extern s32 sceRead(s32 fileHandle, s32 buffer, s32 byteCount);

extern char D_003B4578[];

/* Read a whole named file into the loader's allocated EE buffer, then close it. */
void sdfCdImageLoadDir(const char *fileName) {
    s32 fileHandle = sceOpen(fileName, SDF_DEV_FILE_OPEN_READ);
    s32 fileBytes;
    s32 bufferAddress;

    if (fileHandle < 0) {
        FatalError(D_003B4578, fileName);
    }
    fileBytes = sceLseek(fileHandle, 0, SDF_DEV_SEEK_END);
    sceLseek(fileHandle, 0, SDF_DEV_SEEK_START);
    bufferAddress = (s32)malloc(fileBytes);
    D_003BDA44 = bufferAddress;
    sceRead(fileHandle, bufferAddress, fileBytes);
    sceClose(fileHandle);
    FlushCache(0);
}

extern u32 D_003BDA3C;

extern void sceCdStInit(s32, s32, s32);

extern void sdfCdImageLoadDir(const char *fileName);

/* Record the disc search path, preload a separate file, and initialize an aligned IOP buffer. */
void sdfCdImageInit(const char *discFileName, const char *preloadFileName) {
    u32 discFileRecord[SDF_DEV_DISC_FILE_WORDS];
    s32 iopHeapAddress;

    dds3ImgPath = (u32)discFileName;
    sdfCdImageLoadDir(preloadFileName);
    if (sceCdSearchFile(discFileRecord, (s32)discFileName) == 0) {
        FatalError(D_003B4578, discFileName);
    }
    D_003BDA40 = discFileRecord[0];
    iopHeapAddress = sceSifAllocIopHeap(SDF_DEV_IOP_BUFFER_BYTES);
    D_003BDA3C = iopHeapAddress;
    sceCdStInit(0x50, 5, (iopHeapAddress + SDF_DEV_IOP_ALIGNMENT_BIAS) & SDF_DEV_IOP_ALIGNMENT_MASK);
}

void sdfCdInit(void) {
    sdfCdImageFd = sdfCreateSema(1, 0xff, 0);
    sdfDiscRequestSemaphore = sdfCreateSema(0, 0xff, 0);
    sdfDiscRequestPending = 0;
    sceCdInit(0);
    sceCdMmode(sdfCdTrayMode);
}

/* Service queued host-file requests for one device worker. */

INCLUDE_RODATA(const s32, "sdf/sdfCd", D_003B4578);

INCLUDE_SDATA(const s32, "sdf/sdfCd", sdfDiscRequestPending);

INCLUDE_SDATA(const s32, "sdf/sdfCd", D_003BD3E0);

INCLUDE_SDATA(const s32, "sdf/sdfCd", sdfCdTrayMode);

