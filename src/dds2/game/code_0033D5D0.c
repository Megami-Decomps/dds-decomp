#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
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
/* Declared before first use (were implicit int). */
extern void sceCdInit(s32);
extern s32 sdfFileCmdFileSize(sdfFileCmdHandle_t *state);
/* Original-style implicit int, kept: matched code depends on it: iSignalSema, sdfFileCmdSeek, sdfDevSetFallbackDeviceWorker. */

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

#define SDF_DEV_RPC_QUEUE_BYTES 0x20

#define SDF_DEV_RPC_SERVER_BYTES 0x50

#define SDF_DEV_RPC_SERVER_ID 0x32647270

#define SDF_DEV_RPC_CLIENT_ID 0x646E7270

#define SDF_DEV_RPC_THREAD_STACK_BYTES 0x1000

#define SDF_DEV_RPC_THREAD_PRIORITY 0x4C

#define SDF_DEV_RPC_BIND_POLL_TICKS 4

#define SDF_DEV_DISC_FILE_WORDS 12

#define SDF_DEV_DISC_SECTOR_BYTES 0x800

#define SDF_DEV_DISC_SECTOR_SHIFT 11

#define SDF_DEV_DISC_ALIGNMENT_MASK 15

#define SDF_DEV_IOP_BUFFER_BYTES 0x28010

#define SDF_DEV_IOP_ALIGNMENT_BIAS 15

#define SDF_DEV_IOP_ALIGNMENT_MASK -16

#define SDF_DEV_REPLY_SEMAPHORE_LIMIT 0x80

extern u8 sdfFileCmdThreadPrioCheckCounter;

extern u32 D_004391A4;

extern s32 sdfDiscRequestPending;

extern u32 sdfDiscRequestSemaphore;

extern SdfDiscFileRecord *sdfCdImageCurrFile;

extern u32 sdfCdImageFd;

extern u32 sdfFileLengthWork;

extern u32 sdfFileSeekWork;

extern u32 sdfDevConsColorTable[];

extern char sdfFileHostRootPath[];


extern s32 sdfFileCmdThreadPrio;

extern DevWorkerEntry sdfFileDeviceCB[];



extern void sceDevctl(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);


extern u8 D_00438B50[];

extern u8 D_00438B58[];






extern u8 sdfPrintRpcBuff[];

extern u32 sdfDiscType;


void sdfDevWaitForDisc(void);

extern s32 dds3ImgPath;

extern void sceCdDiskReady(s32);

extern s32 sceCdGetDiskType(void);


extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path,
                                void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32), s32 callbackContext);

extern s32 sdfFileCmdAnalyzeFileName(const char *path, char **outResolvedPath);

extern void sdfFileCmdAllocHandle(sdfFileCmdHandle_t *state);

extern sdfFileCmdHandle_t *sdfFileCmdNewHandle(char *resolvedPath, s32 workerIndex, s32 operation,
                                void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32), s32 callbackContext);

extern u16 sdfFileCmdDefaultCreateMode;
extern void sceCdMmode(u32 discType);

extern s32 sdfFileSemaId;

extern char D_0042E3A0[]; /* "cdrom0:\\IRX\\DEV9.IRX;1 resident fail.\n", followed by padding no C emits */

extern u8 sdfDev9LoadFlag;

extern u8 D_00438B68;

extern s32 sceSifLoadStartModule(const char *modulePath, s32 flags, void *moduleArguments, s32 *resident);

#include "sdf_debug_api.h"

extern void FatalError(const char *format, ...) __attribute__((noreturn));

extern char D_0042E288[];

extern u32 D_0043919C;

extern u32 D_004391A0;


extern void sceCdStInit(s32, s32, s32);

extern void sdfCdImageLoadDir(const char *fileName);

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_00438AF0[];



/* SifCommand is the complete 0x10-byte input value. Console rendering uses
 * source/end as x/y storage, argument as depth bits, and command as RGBA.
 * Its RPC source/end interpretation remains unchanged. */
extern void *sdfDevVprintf(SifCommand *input, const char *format, void *args);

extern void sdfDevConsInit(void);


struct sdfTexHandle_t;


extern u32 sdfDevConsFontUvTable[96];

/* Cached texture keeps the provider's opaque pointer/address boundary. */
extern u32 sdfDebugFontHandle;

void *sdfDevVprintf(SifCommand *input, const char *format, void *arguments) {
    char text[0x200];
    s32 formattedCount;
    u8 *start;
    u8 *cursor;
    SdfConsolePacketHeader *header;
    u8 *textCursor;
    s32 character;
    s32 glyphCount;
    u32 color;
    u64 position;
    s32 quadwordCount;

    sdfDevConsInit();
    formattedCount = vsprintf(text, format, arguments);
    start = (u8 *)sdfGrOpenPacketBuff();
    if (formattedCount == 0) {
        *(u128 *)start = 0;
        cursor = start + 0x10;
    } else {
        header = (SdfConsolePacketHeader *)start;
        cursor = start + sizeof(*header);
        glyphCount = 0;
        /* This dual-use command carries x, y, depth and RGBA in its four words. */
        position = (u32)((u16)input->source | ((u32)(u16)input->end << 16)) |
                   ((u64)(u32)input->argument << 32);
        color = input->command;
        textCursor = (u8 *)text;
        while ((character = *textCursor++) != 0) {
            if (character >= 0x20) {
                SdfConsoleGlyph *glyph = (SdfConsoleGlyph *)cursor;
                u32 uv = sdfDevConsFontUvTable[character - 0x20];

                glyph->color = color;
                glyph->firstUv = uv;
                glyph->firstPosition = position;
                glyph->secondUv = (u32)(uv + 0x00C000C0);
                glyph->secondPosition = position + 0x006000C0;
                cursor += sizeof(*glyph);
                glyphCount++;
            }
            /* Control bytes consume the same horizontal advance as glyphs. */
            position += 0xC0;
        }
        if (glyphCount & 1) {
            *(u64 *)cursor = 0;
            cursor += sizeof(u64);
        }
        quadwordCount = ((s32)(cursor - start) >> 4) - 1;
        input->source = (u16)position;
        header->quadwordCount = quadwordCount;
        header->vifNop = 0;
        header->vifDirect = 0x50000000 | quadwordCount;
        header->setupTag = 0x10AB400000000003ULL;
        header->setupRegisters = 0xEEE;
        header->textureClamp = 0;
        header->textureClampRegister = 8;
        header->textureFilter = 1;
        header->textureFilterRegister = 0x14;
        header->textureState = sdfTexGetTex0Reg((struct sdfTexHandle_t *)sdfDebugFontHandle);
        header->textureStateRegister = 6;
        header->glyphTag = 0x5400000000008000ULL | glyphCount;
        header->glyphRegisters = 0x53531;
    }
    sdfGrClosePacketBuff((s32)cursor);
    return start;
}

void *sdfDevPrintfEx(void *packet, const char *fmt, ...) {
    __builtin_va_list args;

    __builtin_stdarg_start(args, fmt);
    return sdfDevVprintf(packet, fmt, args);
}

void *sdfDevPrintf(s32 source, s32 end, s32 argument, s32 index, const char *fmt, ...) {
    SifCommand packet;
    __builtin_va_list args;

    sdfPktInit(&packet, source, end, argument, index);
    __builtin_stdarg_start(args, fmt);
    return sdfDevVprintf(&packet, fmt, args);
}

void sdfPktSetCmd(SifCommand *packet, s32 index) {
    packet->command = sdfDevConsColorTable[index];
}

void sdfPktInit(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index) {
    packet->source = source;
    packet->end = end;
    packet->argument = argument;
    sdfPktSetCmd(packet, index);
}

/* Copy a positive source/end span into the RPC reply buffer; otherwise return NULL. */
void *sdfPrintRpcFunc(s32 unused, SifCommand *packet) {
    s32 copyBytes = packet->end - packet->source;

    if (copyBytes > 0) {
        memcpy(packet, (void *)packet->source, copyBytes);
        return packet;
    }
    return NULL;
}

/* Register the EE-side buffer service and run its RPC queue on the current thread. */
void sdfPrintRpcThread(void) {
    u8 rpcQueue[SDF_DEV_RPC_QUEUE_BYTES];
    u8 rpcServer[SDF_DEV_RPC_SERVER_BYTES];
    sceSifSetRpcQueue(rpcQueue, GetThreadId());
    sceSifRegisterRpc(rpcServer, SDF_DEV_RPC_SERVER_ID, sdfPrintRpcFunc, sdfPrintRpcBuff, 0, 0, rpcQueue);
    sceSifRpcLoop(rpcQueue);
}

extern SifRpcClientContextView sdfDevRpcClient;

extern s8 D_00438AB8;

extern char D_00438AC0[];

extern s32 scePrintf(const char *, ...);




/* Start the RPC server thread and bind to the remote service, polling every 4 timer ticks. */
void sdfStartDevRpcServerAndBindClient(void) {
    s32 waitStart;

    _StartThread(sdfCreateThreadWithStack(sdfPrintRpcThread, SDF_DEV_RPC_THREAD_STACK_BYTES, SDF_DEV_RPC_THREAD_PRIORITY), 0);
    while (sceSifMBindRpc(&sdfDevRpcClient.client, SDF_DEV_RPC_CLIENT_ID, 0) >= 0) {
        if (sdfDevRpcClient.client.server != NULL) {
            D_00438AB8 = 1;
            break;
        }
        waitStart = sdfGetRunTimeCountBase();
        while ((s32)sdfGetRunTimeCount(waitStart) < SDF_DEV_RPC_BIND_POLL_TICKS) {
        }
    }
}

/* Print text through the bound IOP dev client, or locally when it is unbound. */
void sdfPuts(const char *text) {
    u8 buffer[0x13F];
    u8 *alignedBuffer;
    s32 length;

    if (D_00438AB8 != 0) {
        length = strlen(text);
        if (length != 0) {
            alignedBuffer = (u8 *)(((u32)&buffer[0x3F]) & ~0x3F);
            memcpy(alignedBuffer, text, length);
            sceSifMCallRpc(&sdfDevRpcClient.client, 0x7D1, 0, alignedBuffer, length, NULL, 0, NULL, NULL);
        }
    } else {
        scePrintf(D_00438AC0, text);
    }
}

s32 sdfPrintf(const char *fmt, ...) {
    char buffer[0x100];
    __builtin_va_list args;
    s32 length;

    __builtin_stdarg_start(args, fmt);
    length = vsprintf(buffer, fmt, args);
    sdfPuts(buffer);
    return length;
}

u32 sdfCdImageGetDirData(void) {
    return D_004391A4;
}

void sdfDevWaitForDisc(void) {
    u8 file[0x30];
    s32 status;
    WaitSema(sdfCdImageFd);
    for (;;) {
        sdfDelayThread(100);
        sceCdDiskReady(0);
        status = sceCdGetDiskType();
        if (sdfDiscType == 2) {
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

extern u8 sdfDiscReadMode;

extern s32 sdfDiscReadPosition;

extern s32 sceCdStStart(s32 arg, u8 *options);


void sdfCdStStart(s32 request) {
    u8 options[4];
    s32 ready;

    options[0] = 0;
    if (sdfDiscReadMode == 1) {
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
    if (sdfDiscReadMode == 1) {
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

extern s32 sceCdStRead(s32 size, s32 buffer, s32 count, s32 *status);

void sdfDevReadDiscUntilComplete(s32 size, s32 buffer) {
    s32 status;
    s32 result;

    for (;;) {
        WaitSema(sdfCdImageFd);
        result = sceCdStRead(size, buffer, 1, &status);
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

extern SdfDiscFileRecord D_00476940;

/* Look up a "cdrom" path through the CD file system: convert it to an ISO
 * name ("\\" separators, upper case, ";1" version), retry the search until
 * the disc answers, and record the file's size in the shared record. */
SdfDiscFileRecord *sdfDevFindDiscFileRecordByCdromPath(u32 pathAddress, u8 *sectorOut, u32 flagAddress) {
    u32 file[9];
    char path[256];
    char *name;
    char *cursor;
    char *flag = (char *)flagAddress;
    s32 character;
    s32 status;
    s32 fd;

    strcpy(path, (char *)pathAddress);
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
        if (sceCdSearchFile(file, (s32)name) != 0) {
            fd = sceOpen(path, 1);
            if (fd < 0) {
                SignalSema(sdfCdImageFd);
                continue;
            }
            D_00476940.size = sceLseek(fd, 0, 2);
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
    *(u32 *)sectorOut = file[0];
    if (flag != NULL) {
        *flag = 1;
    }
    return &D_00476940;
}


extern char D_00438AD0[]; /* cdrom */

extern char D_0042E258[]; /* filename too long : %s */

/* Walk a '/'-separated path through the preloaded disc index: each directory
 * holds its children sorted by name, with a negative size whose magnitude is
 * the child count, so every segment is a binary search. */
SdfDiscFileRecord *sdfCdImageSearchFile(u32 pathAddress, u8 *sectorOut, u32 flagAddress) {
    char name[32];
    char *path = (char *)pathAddress;
    char *flag = (char *)flagAddress;
    char *segment;
    char *cursor;
    SdfDiscFileRecord *entries;
    SdfDiscFileRecord *node;
    u32 base;
    s32 character;
    u32 length;
    s32 low;
    s32 high;
    s32 mid;
    s32 order;

    if (memcmp(path, D_00438AD0, 5) == 0) {
        return sdfDevFindDiscFileRecordByCdromPath(pathAddress, sectorOut, flagAddress);
    }
    base = D_004391A4;
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
            FatalError(D_0042E258, path);
        }
        memcpy(name, segment, length);
        name[length] = 0;
        entries = (SdfDiscFileRecord *)(base + node->field4);
        low = 0;
        high = -node->size;
        for (;;) {
            if (low >= high) {
                return NULL;
            }
            mid = (low + high) >> 1;
            node = &entries[mid];
            order = strcmp(name, (char *)(base + node->field0));
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
    *(u32 *)sectorOut = D_004391A0 + node->field4;
    if (flag != NULL) {
        *flag = 0;
    }
    return node;
}

extern s32 D_004391AC;

extern s32 sdfCdImageCurrActive;

extern u8 *sdfCdImageOffset;

extern u8 sdfDevReadSectorBuffer[SDF_DEV_DISC_SECTOR_BYTES];

extern void sdfCdImageClose(void);

s32 sdfDevOpenDiscFileAndGetSize(const char *name) {
    s32 result;
    s32 request[4];
    SdfDiscFileRecord *file;

    file = sdfCdImageSearchFile((u32)name, (u8 *)request, (u32)&sdfDiscReadMode);
    result = -1;
    if (file != 0) {
        D_004391AC = file->size;
        sdfCdImageCurrFile = file;
        sdfCdImageCurrActive = 0;
        if (name[1] == 0x76 || name[1] == 0x56) {
            sdfDevSeekDiscWithRequestGate(request[0]);
        } else {
            sdfCdStStart(request[0]);
        }
        result = file->size;
    }
    return result;
}

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */
s32 sdfCdImageRead(void *buffer, s32 byteCount) {
    s32 available = D_004391AC;
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
                sdfDevReadDiscUntilComplete(transferBytes >> SDF_DEV_DISC_SECTOR_SHIFT, (s32)destination);
            }
            if (t > 0) {
                sdfDevReadDiscUntilComplete(1, (s32)sdfDevReadSectorBuffer);
                memcpy(destination + transferBytes, sdfDevReadSectorBuffer, t);
                sdfCdImageOffset = sdfDevReadSectorBuffer + t;
                sdfCdImageCurrActive = SDF_DEV_DISC_SECTOR_BYTES - t;
            }
        } else {
            while (transferBytes >= SDF_DEV_DISC_SECTOR_BYTES) {
                sdfDevReadDiscUntilComplete(1, (s32)sdfDevReadSectorBuffer);
                memcpy(destination, sdfDevReadSectorBuffer, SDF_DEV_DISC_SECTOR_BYTES);
                transferBytes -= SDF_DEV_DISC_SECTOR_BYTES;
                destination += SDF_DEV_DISC_SECTOR_BYTES;
            }
            if (transferBytes > 0) {
                sdfDevReadDiscUntilComplete(1, (s32)sdfDevReadSectorBuffer);
                memcpy(destination, sdfDevReadSectorBuffer, transferBytes);
                sdfCdImageOffset = sdfDevReadSectorBuffer + transferBytes;
                sdfCdImageCurrActive = SDF_DEV_DISC_SECTOR_BYTES - transferBytes;
            }
        }
    }
    D_004391AC = available - byteCount;
    if (D_004391AC == 0) {
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

u32 sdfCdImageCheckExist(index)
    u32 index;
{
    SdfDiscFileRecord *record;
    u8 buffer[16];

    record = sdfCdImageSearchFile(index, buffer, 0);
    return record != 0;
}

s32 sdfCdImageGetFileSize(u32 index) {
    u8 buffer[16];
    SdfDiscFileRecord *record;

    record = sdfCdImageSearchFile(index, buffer, 0);
    if (record != 0) {
        return record->size;
    }
    return -1;
}

s32 sdfCdImageGetCurrFileSize(void) {
    if (sdfCdImageCurrFile == 0) {
        FatalError("file didn't open.");
    }
    return sdfCdImageCurrFile->size;
}






/* Read a whole named file into the loader's allocated EE buffer, then close it. */
void sdfCdImageLoadDir(const char *fileName) {
    s32 fileHandle = sceOpen(fileName, SDF_DEV_FILE_OPEN_READ);
    s32 fileBytes;
    s32 bufferAddress;

    if (fileHandle < 0) {
        FatalError(D_0042E288, fileName);
    }
    fileBytes = sceLseek(fileHandle, 0, SDF_DEV_SEEK_END);
    sceLseek(fileHandle, 0, SDF_DEV_SEEK_START);
    bufferAddress = malloc(fileBytes);
    D_004391A4 = bufferAddress;
    sceRead(fileHandle, bufferAddress, fileBytes);
    sceClose(fileHandle);
}

/* Record the disc search path, preload a separate file, and initialize an aligned IOP buffer. */
void sdfCdImageInit(const char *discFileName, const char *preloadFileName) {
    u32 discFileRecord[SDF_DEV_DISC_FILE_WORDS];
    s32 iopHeapAddress;

    dds3ImgPath = (u32)discFileName;
    sdfCdImageLoadDir(preloadFileName);
    if (sceCdSearchFile(discFileRecord, (s32)discFileName) == 0) {
        FatalError(D_0042E288, discFileName);
    }
    D_004391A0 = discFileRecord[0];
    iopHeapAddress = sceSifAllocIopHeap(SDF_DEV_IOP_BUFFER_BYTES);
    D_0043919C = iopHeapAddress;
    sceCdStInit(0x50, 5, (iopHeapAddress + SDF_DEV_IOP_ALIGNMENT_BIAS) & SDF_DEV_IOP_ALIGNMENT_MASK);
}

void sdfInitDeviceSemaphores(void) {
    sdfCdImageFd = sdfCreateSema(1, 0xff, 0);
    sdfDiscRequestSemaphore = sdfCreateSema(0, 0xff, 0);
    sdfDiscRequestPending = 0;
    sceCdInit(0);
    sceCdMmode(sdfDiscType);
}

extern char sdfDiscPathPrefix[];

/* Convert a relative disc path to the drive's uppercase backslash form with ;1 suffix. */
void sdfFileSetCdFilename(char *dst, char *src) {
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

char *sdfFileSetHostFilename(char *dst, char *src) {
    if (*src == 0x2F) {
        strcpy(dst, sdfFileHostRootPath);
        return strcat(dst, src);
    }
    *(Bytes7 *)dst = D_00438AF0[0];
    return strcat(dst, src);
}

typedef struct Bytes6 {
    s8 b[6];
} Bytes6;

extern Bytes6 sdfPfsPathPrefix[];

void sdfFileSetHddFilename(char *dst, char *src) {
    *(Bytes6 *)dst = sdfPfsPathPrefix[0];
    strcat(dst, src);
}

/* Retail disables the implicit-disc path probe used by the ordered device dispatch. */
u32 sdfFileChkCdFile(char *path) {
    return 0;
}

/* Retail disables the implicit-host probe; the host builder is still used explicitly. */
u32 sdfFileChkHostFile(char *path) {
    return 0;
}

/* Retail disables the implicit-PFS probe before the configured-device fallback. */
u32 sdfFileChkHddFile(char *path) {
    return 0;
}

extern char D_00438B00[]; /* cdrom */

extern u8 D_00438AE0;



/* Return an allocated device path, normalizing disc-path case and separators. */
char *sdfFileMakeFilename(char *path) {
    s32 length = strlen(path);
    char *result = sdfChipAllocDirect(0x80);
    char *cursor;
    s32 character;
    if (strchr(path, ':') != NULL) {
        memcpy(result, path, length);
        result[length] = 0;
        if (strncmp(result, D_00438B00, 5) == 0) {
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
        switch (D_00438AE0) {
        case 0: sdfFileSetCdFilename(result, path); break;
        case 2: sdfFileSetHddFilename(result, path); break;
        case 1: sdfFileSetHostFilename(result, path); break;
        default: strcpy(result, path); break;
        }
    }
    return result;
}

/* Capture seek/size replies, then wake the synchronous command waiter. */
void sdfFileCallback(sdfFileCmdHandle_t *state, s32 event, s32 unused, s32 value, s32 callbackContext) {
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

extern char *sdfFileMakeFilename();



s32 sdfFileCheckExist(char *path) {
    char *resolved = sdfFileMakeFilename(path);
    s32 fd;

    if (*resolved == '/') {
        return sdfCdImageCheckExist((u32)resolved);
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
    sdfFileCmdFileSize(state);
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

    ((DevPathPrefix *)sdfFileHostRootPath)->head = ((DevPathPrefix *)D_00438AF0)->head;
    ((DevPathPrefix *)sdfFileHostRootPath)->tail = ((DevPathPrefix *)D_00438AF0)->tail;
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

extern void sdfDevSetFallbackDeviceWorker(s32 worker);

/* Select the default path-construction worker and the no-prefix fallback worker. */
void sdfFileSetDefaultSrc(s32 worker) {
    D_00438AE0 = worker;
    sdfDevSetFallbackDeviceWorker(worker);
}

extern u8 D_00438B1E;

/* Select the worker used when a file path has no explicit device prefix. */
void sdfDevSetFallbackDeviceWorker(s32 worker) {
    D_00438B1E = worker;
}


extern void sdfFileCmdStartThread(s32 index);

extern sdfFileCmdHandle_t *D_00438B08;

extern sdfFileCmdHandle_t *D_00438B0C;

extern s16 D_00438B1C;


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
    state->previous = D_00438B0C;
    if (D_00438B0C != NULL) {
        D_00438B0C->next = state;
    } else {
        D_00438B08 = state;
    }
    last = worker->last;
    D_00438B0C = state;
    state->workerPrev = last;
    if (last != NULL) {
        last->workerNext = state;
    } else {
        worker->first = state;
        wake = 1;
    }
    worker->last = state;
    D_00438B1C += 1;
    if (interrupts != 0) {
        EIntr();
    }
    if (wake != 0) {
        SignalSema(worker->semaphore);
    }
}

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

    fileHandle = state->resourceId;
    state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
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
        state->callback(state, SDF_DEV_EVENT_INACTIVE, 0, 0, state->callbackContext);
    }
}


/* Service queued host-file requests for one device worker. */
INCLUDE_RODATA(const s32, "game/code_0033D5D0", D_0042E288);

void sdfFileCmdThread(DevWorkerEntry *worker) {
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
            state->resourceId = fileHandle;
            state->transferred = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_OPENED, 0, 0, state->callbackContext);
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
            state->resourceId = fileHandle;
            state->transferred = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_OPENED, 0, 0, state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_SEEK - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_SEEKING;
            endOffset = sceLseek(state->resourceId, state->operationArg, state->options);
            if (endOffset < 0) {
                sdfFileCmdFinish(state, endOffset);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred = endOffset;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_SEEK_REPLY, 0, endOffset, state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_GET_SIZE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_SEEKING;
            position = sceLseek(state->resourceId, 0, SDF_DEV_SEEK_CURRENT);
            if (position < 0) {
                sdfFileCmdFinish(state, position);
                continue;
            }
            endOffset = sceLseek(state->resourceId, 0, SDF_DEV_SEEK_END);
            if (endOffset < 0) {
                sdfFileCmdFinish(state, endOffset);
                continue;
            }
            result = sceLseek(state->resourceId, position, SDF_DEV_SEEK_START);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred = endOffset;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_SIZE_REPLY, 0, endOffset, state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_READ - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_READING;
            result = sceRead(state->resourceId, (s32)state->requestData,
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
                                state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_WRITE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_WRITING;
            result = sceWrite(state->resourceId, (s32)state->requestData,
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
                                state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_CLOSE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_CLOSING;
            result = sceClose(state->resourceId);
            state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            sdfFileCmdDequeue(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_CLOSED, 0, 0, state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_READ_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = sceOpen(state->resolvedPath, SDF_DEV_FILE_OPEN_READ, 0);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->resourceId = fileHandle;
            transferResult = state->requestExtra;
            /* A negative count means the whole file; NULL storage is allocated below. */
            if (transferResult < 0) {
                state->state = SDF_DEV_STATE_SEEKING;
                transferResult = sceLseek(fileHandle, 0, SDF_DEV_SEEK_END);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
                result = sceLseek(state->resourceId, 0, SDF_DEV_SEEK_START);
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
                transferResult = sceRead(state->resourceId, (s32)buffer, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            state->state = SDF_DEV_STATE_CLOSING;
            sceClose(state->resourceId);
            state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
            sdfFileCmdDequeue(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_BUFFER_REPLY, (s32)state->requestData, transferResult,
                         state->callbackContext);
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
            state->resourceId = fileHandle;
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
        sceClose(state->resourceId);
        state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
        sdfFileCmdDequeue(state);
        state->state = SDF_DEV_STATE_COMPLETE;
        callback = state->callback;
        if (callback != NULL) {
            callback(state, SDF_DEV_EVENT_BUFFER_REPLY, (s32)state->requestData, transferResult,
                            state->callbackContext);
        }
        SignalSema(worker->semaphore);
    }
}

/* Worker slot 3 services disc-backed requests through serialized disc operations. */
void sdfFileCdImageCmdThread(DevWorkerEntry *worker) {
    sdfFileCmdHandle_t *state;
    void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32);
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
                callback(state, SDF_DEV_EVENT_OPENED, 0, 0, state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_GET_SIZE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_ACTIVE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_SIZE_REPLY, 0, sdfCdImageGetCurrFileSize(),
                                state->callbackContext);
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
                                state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_CLOSE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_CLOSING;
            sdfCdImageClose();
            sdfFileCmdDequeue(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_CLOSED, 0, 0, state->callbackContext);
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
            state->resourceId = -1;
            sdfFileCmdDequeue(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_BUFFER_REPLY, (s32)state->requestData, transferResult,
                                state->callbackContext);
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
char *sdfFileCmdMakeRealFileName(char *path, s32 worker) {
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
s32 sdfFileCmdAnalyzeFileName(const char *path, char **resolvedPath) {
    s32 worker;
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
    *resolvedPath = sdfFileCmdMakeRealFileName((char *)name, worker);
    return worker;
}

/* Allocate a request state and retain the callback's opaque context word. */
sdfFileCmdHandle_t *sdfFileCmdNewHandle(char *resolvedPath, s32 workerIndex, s32 operation,
                        void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32), s32 callbackContext) {
    sdfFileCmdHandle_t *state = (sdfFileCmdHandle_t *)sdfAllocAndClearQuadwords(SDF_DEV_STATE_BYTES);
    state->resolvedPath = resolvedPath;
    state->workerIndex = workerIndex;
    state->operation = operation;
    state->state = 0;
    state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
    state->callback = callback;
    state->callbackContext = callbackContext;
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
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_OPEN_READ, callback, callbackContext);
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
    state->operationArg = seekOffset;
    state->options = seekOrigin;
    state->operation = SDF_DEV_OPERATION_SEEK;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return 0;
}

/* Query file size without changing the file position; return -1 unless active. */
s32 sdfFileCmdFileSize(sdfFileCmdHandle_t *state) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->operation = SDF_DEV_OPERATION_GET_SIZE;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return 0;
}

/* Queue a byte-counted read; return -1 without changing an inactive request. */
s32 sdfFileCmdRead(sdfFileCmdHandle_t *state, void *readBuffer, s32 byteCount) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->requestExtra = byteCount;
    state->operation = SDF_DEV_OPERATION_READ;
    state->requestData = readBuffer;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return 0;
}

/* Queue a byte-counted write; return -1 without changing an inactive request. */
s32 sdfFileCmdWrite(sdfFileCmdHandle_t *state, const void *buffer, s32 byteCount) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->requestExtra = byteCount;
    state->operation = SDF_DEV_OPERATION_WRITE;
    state->requestData = (void *)buffer;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return 0;
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

/* Active state and close operation share an ID; retain the original byte assignment. */
s32 sdfFileCmdClose(sdfFileCmdHandle_t *request) {
    SdfDevStateCode requestState = request->state;

    if (requestState != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    request->operation = requestState;
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
                        void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32), s32 callbackContext) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_READ_BUFFER, callback, callbackContext);
    state->requestExtra = byteCount;
    state->requestData = buffer;
    state->operationArg = 0;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Enqueue a one-shot write using the supplied buffer, byte count and creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteFile(const char *path, void *buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                        s32 callbackContext, s32 options) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_WRITE_BUFFER, callback, callbackContext);
    state->requestExtra = byteCount;
    state->requestData = buffer;
    state->options = options != 0 ? options : sdfFileCmdDefaultCreateMode;
    state->operationArg = 0;
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

void sdfPowerOffLoop(s32 semaphore) {
    s32 status;

    for (;;) {
        WaitSema(semaphore);
        sceDevctl(D_00438B50, 0x5003, 0, 0, 0, 0);
        sceDevctl(D_00438B58, 0x4806, 0, 0, 0, 0);
        sceCdPowerOff(&status);
    }
}


void sdfPowerOffInterruptCallback(void *semaphore) {
    iSignalSema((s32)semaphore);
}

extern u8 sdfHddValid;

extern char sdfDevHddModuleArguments[];

extern char sdfDevPfsModuleArguments[];

extern char D_00477180[];

extern char D_00438B60[];

extern void sdfDev9LoadModule(void);

extern s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

typedef void (*SdfPowerOffCallback)(void *);

extern SdfPowerOffCallback sceCdPOffCallback(SdfPowerOffCallback, void *);

extern s32 sceFsInit(void);


extern s32 sceMount(const char *, const char *, s32, const void *, s32);

/* Load the HDD/PFS dev modules and mount path before enabling PFS debug mode. */
void sdfDevInitializeHddPfsDebugMode(char *path) {
    char buffer[0x100];
    s32 resident;
    s32 result;
    s32 semaphore;
    s32 thread;

    sdfHddValid = 0;
    sdfDev9LoadModule();
    if (D_00438B68 == 0) {
        return;
    }
    result = sceSifLoadStartModule("cdrom0:\\IRX\\ATAD.IRX;1", 0, NULL, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    result = sceSifLoadStartModule("cdrom0:\\IRX\\HDD.IRX;1", 0xC, sdfDevHddModuleArguments, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    result = sceSifLoadStartModule("cdrom0:\\IRX\\PFS.IRX;1", 0x12, sdfDevPfsModuleArguments, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    semaphore = sdfCreateSema(0, 1, 0);
    thread = CreateThreadSimple((void *)sdfPowerOffLoop, D_00477180, 0x800, 1);
    _StartThread(thread, semaphore);
    sceCdPOffCallback(sdfPowerOffInterruptCallback, (void *)semaphore);
    sceFsInit();
    sprintf(buffer, "hdd0:%s,", path);
    sceMount(D_00438B60, buffer, 0, 0, 0);
    sdfHddValid = 1;
}

void sdfDev9LoadModule(void) {
    s32 resident;
    if (sdfDev9LoadFlag == 0) {
        D_00438B68 = 0;
        if (sceSifLoadStartModule("cdrom0:\\IRX\\DEV9.IRX;1", 0, NULL, &resident) < 0) {
            printf("cdrom0:\\IRX\\DEV9.IRX;1 could't load.\n");
            return;
        }
        if (resident != 0) {
            printf(D_0042E3A0);
            return;
        }
        D_00438B68 = 1;
        sdfDev9LoadFlag = 1;
    }
}

/* Writable device directory; its suffix is replaced by the path setter. */
char sdfFileHostRootPath[64] = "host0:";

INCLUDE_RODATA(const s32, "game/code_0033D5D0", D_0042E3A0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AB8);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AC0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", sdfDiscRequestPending);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AD0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", sdfDiscType);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AE0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", sdfFileSemaId);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", sdfDiscPathPrefix);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438AF0);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", sdfPfsPathPrefix);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B00);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B08);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B0C);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B10);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B14);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B18);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B1C);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B1E);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", sdfFileCmdThreadPrioCheckCounter);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", sdfFileCmdThreadPrio);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", sdfFileCmdDefaultCreateMode);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B28);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B30);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B38);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B40);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B48);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B50);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B58);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B60);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B68);

INCLUDE_SDATA(const s32, "game/code_0033D5D0", D_00438B70);

