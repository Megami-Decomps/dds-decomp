#include "common.h"
#include "sdf_draw.h"

#define SDF_DEV_WORKER_COUNT 4
#define SDF_DEV_DEFAULT_PRIORITY 0x48
#define SDF_DEV_OVERRIDE_PRIORITY 0x78
#define SDF_DEV_PRIORITY_OVERRIDE_TICKS 3

/* One-based request IDs; host-file dispatch subtracts the first ID. */
#define SDF_DEV_OPERATION_NONE 0
#define SDF_DEV_OPERATION_OPEN_READ 1
#define SDF_DEV_OPERATION_OPEN_WRITE 2
#define SDF_DEV_OPERATION_SEEK 3
#define SDF_DEV_OPERATION_GET_SIZE 4
#define SDF_DEV_OPERATION_READ 5
#define SDF_DEV_OPERATION_WRITE 6
#define SDF_DEV_OPERATION_CLOSE 7
#define SDF_DEV_OPERATION_READ_BUFFER 8
#define SDF_DEV_OPERATION_WRITE_BUFFER 9

#define SDF_DEV_EVENT_INACTIVE 0
#define SDF_DEV_EVENT_OPENED 2
#define SDF_DEV_EVENT_SEEK_REPLY 3
#define SDF_DEV_EVENT_SIZE_REPLY 4
#define SDF_DEV_EVENT_READ_REPLY 5
#define SDF_DEV_EVENT_WRITE_REPLY 6
#define SDF_DEV_EVENT_CLOSED 7
#define SDF_DEV_EVENT_BUFFER_REPLY 8

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

#define SDF_BCD_DIGIT_BITS 4
#define SDF_DECIMAL_RADIX 10

/* Keep the retail single-precision values; do not round or recompute them. */
#define SDF_TRIG_INVERSE_TAU 0.15915494f
#define SDF_TRIG_HALF_PI 1.5707963f
#define SDF_TRIG_PI 3.1415926f
#define SDF_TRIG_TAU 6.2831852f
#define SDF_SINE_POLYNOMIAL_SCALE 3.9999996f
#define SDF_ASIN_SAMPLE_COUNT 128

extern char D_003B4690[]; /* "cdrom0:\\IRX\\DEV9.IRX;1 resident fail.\n", followed by padding no C emits */

typedef struct SifCommand {
    s32 source;  /* 0x0 */
    s32 end;     /* 0x4 */
    s32 argument; /* 0x8 */
    u32 command; /* 0xC */
} SifCommand;


typedef struct DevState {
    struct DevState *next; /* 0x0 */
    struct DevState *previous; /* 0x4 */
    struct DevState *workerNext; /* 0x8 */
    struct DevState *workerPrev; /* 0xC */
    void *resource; /* 0x10 */
    u8 workerIndex; /* 0x14 */
    u8 operation; /* 0x15 */
    s8 state; /* 0x16 */
    u8 pad17; /* 0x17 */
    s32 operationArg; /* 0x18 */
    s32 requestExtra; /* 0x1C */
    void *requestData; /* 0x20 */
    s32 options; /* 0x24 */
    s32 resourceId; /* 0x28 */
    s32 result; /* 0x2C */
    s32 transferred; /* 0x30 */
    u8 pad34[4]; /* 0x34 */
    void (*callback)(struct DevState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4); /* 0x38 */
    s32 callbackContext; /* 0x3C */
} DevState;

#define SDF_DEV_STATE_OPENING 2
#define SDF_DEV_STATE_SEEKING 3
#define SDF_DEV_STATE_READING 4
#define SDF_DEV_STATE_WRITING 5
#define SDF_DEV_STATE_CLOSING 6
#define SDF_DEV_STATE_ACTIVE 7
#define SDF_DEV_STATE_COMPLETE 8
#define SDF_DEV_STATE_INACTIVE 9

/* Native 0x18 worker record; semaphore-only addresses point inside this array,
 * not at a separately allocated table. */
typedef struct DevWorkerEntry {
    s32 threadId; /* 0x00 */
    s32 semaphore; /* 0x04 */
    struct DevState *first; /* 0x08 */
    struct DevState *last; /* 0x0C */
    u8 pad10[8];
} DevWorkerEntry;

extern u8 sdfDevicePriorityOverrideTicks;
extern u8 D_003BD42E;
extern u8 D_003BD3F0;
extern s16 D_003BD420;
extern DevState *D_003BD424;
extern DevState *D_003BD428;
extern s32 sdfDeviceWorkerPriority;

extern u32 sdfDevOperationReplyValue;

extern u32 sdfDevControlReplyValue;

extern s32 sdfDevReplySemaphore;

extern s32 func_002E5158(u32, u8 *, u32);
extern void func_002E5D98(s32 arg0);

extern s32 sdfOpenDiscFileRecord;
extern u32 sdfDiscSemaphore;

extern s32 sdfDiscRequestPending;
extern u32 sdfDiscRequestSemaphore;

extern u32 D_003BDA44;

extern u32 D_003987E0[];
extern char D_00398820[];
extern DevWorkerEntry sdfDeviceWorkerEntries[];
extern u8 sdfPfsPathPrefix[];
extern f32 sdfNormalizedAsinSamples[];
extern char *func_002E5970(char *path);

extern s32 SignalSema(s32 sema);
extern s32 WaitSema(s32 sema);
extern s32 ChangeThreadPriority(s32 tid, s32 prio);
extern void sdfDevUnlinkAndFreeState(DevState *arg0);
extern void sdfDevRecycleCompletedState(DevState *arg0);
extern DevState *sdfDevCreateCallbackState(const char *path,
                                void (*callback)(DevState *, s32, s32, s32, s32), s32 arg2);
extern s32 func_0030EB78(s32 arg0);
extern s32 func_00312C08(DevState *arg0);
extern void func_003110C8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void EIntr(void);
extern void sceCdPowerOff(void *arg0);
extern s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);
extern void sdfPanicHaltPrintf(const char *arg0, ...) __attribute__((noreturn));
extern void *sdfAllocSizeClassBlock(s32 size);
extern void sdfReleaseChipBlock(void *ptr);
extern void *sdfAllocAndClearQuadwords(s32 size);
extern s32 sdfAllocGeneralBlock(s32 size);
extern void func_002D0750(s32 arg0, s32 arg1);
extern s32 sdfResourceRetainAddress(s32 arg0);
extern void sdfDecrementAllocationReferenceCount(s32 arg0);
extern u32 strlen(const char *s);
extern void func_002F4190(u32 arg0);
extern u32 sdfDiscType;
extern u8 D_003BD460[];
extern u8 D_003BD468[];
extern s32 GetThreadId(void);
extern void sceSifSetRpcQueue(void *, s32);
extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);
extern void sceSifRpcLoop(void *);
extern u8 sdfDevRpcBuffer[];
extern void sdfSleepWithAlarm(s32);
void sdfDevWaitForDisc(void);
extern s32 sdfDiscLoadFilename;
extern void func_002F3F98(s32);
extern s32 func_002F4258(void);
extern s32 sceCdSearchFile(void *, s32);
extern u16 sdfDefaultDevRequestOptions;
extern s32 func_002E69F0(const char *, void **);
extern void sdfDevEnqueueStateAndWakeWorker(DevState *);
extern DevState *sdfDevAllocState(void *, s32, s32,
                                void (*)(DevState *, s32, s32, s32, s32), s32);
extern u8 sdfDevModuleLoaded;
extern u8 D_003BD478;
extern s32 func_00312618(const char *, s32, void *, s32 *);
extern void func_003003F0(const char *);

extern void *func_002E4720(void *packet, const char *format, void *args);

extern s32 func_00305B08(char *dst, const char *fmt, void *args);

extern void func_002E4B80(const char *text);

extern u8 sdfDiscReadMode;

extern s32 sdfDiscReadPosition;

extern s32 func_002F4588(s32 arg, u8 *options);

extern s32 sceCdStatus(void);

extern s32 func_002F4658(s32 size, s32 buffer, s32 arg2, s32 *status);

extern s32 func_002F4620(void);

extern s32 D_003BDA4C;

extern s32 D_003BDA54;
extern u8 *D_003BDA50;
extern u8 D_003FA000[SDF_DEV_DISC_SECTOR_BYTES];
extern void sdfServicePendingOperationUnderSemaphore(void);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4720);

void *sdfFormatSifPacket(void *packet, const char *format, ...) {
    __builtin_va_list args;

    __builtin_stdarg_start(args, format);
    return func_002E4720(packet, format, args);
}

void *sdfCreateFormattedSifCommand(s32 source, s32 end, s32 argument, s32 index, const char *fmt, ...) {
    SifCommand packet;
    __builtin_va_list args;

    sdfPktInit(&packet, source, end, argument, index);
    __builtin_stdarg_start(args, fmt);
    return func_002E4720(&packet, fmt, args);
}

void sdfPktSetCmd(SifCommand *packet, s32 index) {
    packet->command = D_003987E0[index];
}

void sdfPktInit(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index) {
    packet->source = source;
    packet->end = end;
    packet->argument = argument;
    sdfPktSetCmd(packet, index);
}

/* Copy a positive source/end span into the RPC reply buffer; otherwise return NULL. */
void *sdfRpcBufHandler(s32 unused, SifCommand *packet) {
    s32 copyBytes = packet->end - packet->source;

    if (copyBytes > 0) {
        memcpy(packet, (void *)packet->source, copyBytes);
        return packet;
    }
    return NULL;
}

/* Register the EE-side buffer service and run its RPC queue on the current thread. */
void sdfDevStartRpcServer(void) {
    u8 rpcQueue[SDF_DEV_RPC_QUEUE_BYTES];
    u8 rpcServer[SDF_DEV_RPC_SERVER_BYTES];
    sceSifSetRpcQueue(rpcQueue, GetThreadId());
    sceSifRegisterRpc(rpcServer, SDF_DEV_RPC_SERVER_ID, sdfRpcBufHandler, sdfDevRpcBuffer, 0, 0, rpcQueue);
    sceSifRpcLoop(rpcQueue);
}

typedef struct SifClient {
    u8 pad00[0x24];
    void *server; /* 0x24: non-NULL once the bind succeeded */
    u8 pad28[8];
} SifClient;

extern SifClient D_003F9B50;
extern s8 D_003BD3C8;
extern char D_003BD3D0[];
extern s32 func_002F4FD8(SifClient *, s32, s32, void *, s32, void *, s32, void (*)(void *), void *);
extern s32 func_0030C8E0(const char *, ...);
extern s32 sdfCreateThreadWithAllocatedWorkspace();
extern s32 func_002CF930(void);
extern s32 sdfGetElapsedTimerTicks(s32);
extern s32 sceSifMBindRpc(void *, s32, s32);
extern s32 _StartThread(s32, s32);

/* Start the RPC server thread and bind to the remote service, polling every 4 timer ticks. */
void sdfStartDevRpcServerAndBindClient(void) {
    s32 waitStart;

    _StartThread(sdfCreateThreadWithAllocatedWorkspace(sdfDevStartRpcServer, SDF_DEV_RPC_THREAD_STACK_BYTES, SDF_DEV_RPC_THREAD_PRIORITY), 0);
    while (sceSifMBindRpc(&D_003F9B50, SDF_DEV_RPC_CLIENT_ID, 0) >= 0) {
        if (D_003F9B50.server != NULL) {
            D_003BD3C8 = 1;
            break;
        }
        waitStart = func_002CF930();
        while (sdfGetElapsedTimerTicks(waitStart) < SDF_DEV_RPC_BIND_POLL_TICKS) {
        }
    }
}

void func_002E4B80(const char *text) {
    u8 buffer[0x13F];
    u8 *alignedBuffer;
    s32 length;

    if (D_003BD3C8 != 0) {
        length = strlen(text);
        if (length != 0) {
            alignedBuffer = (u8 *)(((u32)&buffer[0x3F]) & ~0x3F);
            memcpy(alignedBuffer, text, length);
            func_002F4FD8(&D_003F9B50, 0x7D1, 0, alignedBuffer, length, NULL, 0, NULL, NULL);
        }
    } else {
        func_0030C8E0(D_003BD3D0, text);
    }
}

s32 sdfPrintFormattedDevMessage(const char *fmt, ...) {
    char buffer[0x100];
    __builtin_va_list args;
    s32 length;

    __builtin_stdarg_start(args, fmt);
    length = func_00305B08(buffer, fmt, args);
    func_002E4B80(buffer);
    return length;
}

u32 sdfDevGetLoadedFileAddress(void) {
    return D_003BDA44;
}

void sdfDevWaitForDisc(void) {
    u8 file[0x30];
    s32 status;
    WaitSema(sdfDiscSemaphore);
    for (;;) {
        sdfSleepWithAlarm(100);
        func_002F3F98(0);
        status = func_002F4258();
        if (sdfDiscType == 2) {
            if (status != 20) {
                continue;
            }
        } else if (status != 18) {
            continue;
        }
        if (sceCdSearchFile(file, sdfDiscLoadFilename) != 0) {
            break;
        }
    }
    SignalSema(sdfDiscSemaphore);
}

void sdfDevSeekDiscRequest(s32 request) {
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
        WaitSema(sdfDiscSemaphore);
        ready = func_002F4588(request, options);
        SignalSema(sdfDiscSemaphore);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
        } else {
            sdfSleepWithAlarm(100);
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
        WaitSema(sdfDiscSemaphore);
        sdfDiscRequestPending = 1;
        WaitSema(sdfDiscRequestSemaphore);
        ready = func_002F4588(request, options);
        SignalSema(sdfDiscSemaphore);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
        } else {
            sdfSleepWithAlarm(100);
        }
    }
    sdfDiscReadPosition = request;
}

void sdfDevReadDiscUntilComplete(s32 size, s32 buffer) {
    s32 status;
    s32 result;

    for (;;) {
        WaitSema(sdfDiscSemaphore);
        result = func_002F4658(size, buffer, 1, &status);
        SignalSema(sdfDiscSemaphore);
        if (status == 0 && result == size) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfDevWaitForDisc();
            sdfDevSeekDiscRequest(sdfDiscReadPosition);
        } else {
            WaitSema(sdfDiscSemaphore);
            func_002F4620();
            SignalSema(sdfDiscSemaphore);
            sdfDevSeekDiscRequest(sdfDiscReadPosition);
        }
    }
    sdfDiscReadPosition += size;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4FB0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5158);

s32 sdfDevOpenDiscFileAndGetSize(const char *name) {
    s32 result;
    s32 request[4];
    s32 file;

    file = func_002E5158((u32)name, (u8 *)request, (u32)&sdfDiscReadMode);
    result = -1;
    if (file != 0) {
        D_003BDA4C = *(s32 *)(file + 8);
        sdfOpenDiscFileRecord = file;
        D_003BDA54 = 0;
        if (name[1] == 0x76 || name[1] == 0x56) {
            sdfDevSeekDiscWithRequestGate(request[0]);
        } else {
            sdfDevSeekDiscRequest(request[0]);
        }
        result = *(s32 *)(file + 8);
    }
    return result;
}

s32 func_002E5398(void *buffer, s32 size) {
    s32 available = D_003BDA4C;
    u8 *destination = buffer;
    s32 bytes;
    s32 transferBytes;
    s32 t;

    if (available <= 0) {
        return 0;
    }
    if (size > available) {
        size = available;
    }
    if (size == 0) {
        return 0;
    }

    bytes = size;
    transferBytes = D_003BDA54;
    if (transferBytes > 0) {
        if (transferBytes > size) {
            transferBytes = size;
        }
        memcpy(destination, D_003BDA50, transferBytes);
        destination += transferBytes;
        D_003BDA50 += transferBytes;
        D_003BDA54 -= transferBytes;
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
                sdfDevReadDiscUntilComplete(1, (s32)D_003FA000);
                memcpy(destination + transferBytes, D_003FA000, t);
                D_003BDA50 = D_003FA000 + t;
                D_003BDA54 = SDF_DEV_DISC_SECTOR_BYTES - t;
            }
        } else {
            while (transferBytes >= SDF_DEV_DISC_SECTOR_BYTES) {
                sdfDevReadDiscUntilComplete(1, (s32)D_003FA000);
                memcpy(destination, D_003FA000, SDF_DEV_DISC_SECTOR_BYTES);
                transferBytes -= SDF_DEV_DISC_SECTOR_BYTES;
                destination += SDF_DEV_DISC_SECTOR_BYTES;
            }
            if (transferBytes > 0) {
                sdfDevReadDiscUntilComplete(1, (s32)D_003FA000);
                memcpy(destination, D_003FA000, transferBytes);
                D_003BDA50 = D_003FA000 + transferBytes;
                D_003BDA54 = SDF_DEV_DISC_SECTOR_BYTES - transferBytes;
            }
        }
    }
    D_003BDA4C = available - size;
    if (D_003BDA4C == 0) {
        sdfServicePendingOperationUnderSemaphore();
    }
    return size;
}

void sdfServicePendingOperationUnderSemaphore(void) {
    if (sdfOpenDiscFileRecord != 0) {
        WaitSema(sdfDiscSemaphore);
        func_002F4620();
        SignalSema(sdfDiscSemaphore);
        sdfOpenDiscFileRecord = 0;
    }
}

s32 sdfPacketExists(u32 request) {
    u8 buffer[16];

    return func_002E5158(request, buffer, 0) != 0;
}

s32 sdfPktQuery(u32 request) {
    u8 buffer[16];
    s32 packet;

    packet = func_002E5158(request, buffer, 0);
    if (packet != 0) {
        return *(s32 *)(packet + 8);
    }
    return -1;
}

s32 sdfDevGetFileSize(void) {
    if (sdfOpenDiscFileRecord == 0) {
        sdfPanicHaltPrintf("file didn't open.");
    }
    return *(s32 *)(sdfOpenDiscFileRecord + 8);
}

extern s32 func_0030E8F0();
extern s32 func_0030ECF8(s32, s32, s32);
extern s32 func_002FF538(s32);
extern s32 func_0030EF30(s32, s32, s32);
extern s32 func_0030F190(s32, s32, s32);
extern void FlushCache(s32);
extern char D_003B4578[];

/* Read a whole named file into the loader's allocated EE buffer, then close it. */
void sdfDevLoadWholeFile(s32 fileNameAddress) {
    s32 fileHandle = func_0030E8F0(fileNameAddress, SDF_DEV_FILE_OPEN_READ);
    s32 fileBytes;
    s32 bufferAddress;

    if (fileHandle < 0) {
        sdfPanicHaltPrintf(D_003B4578, fileNameAddress);
    }
    fileBytes = func_0030ECF8(fileHandle, 0, SDF_DEV_SEEK_END);
    func_0030ECF8(fileHandle, 0, SDF_DEV_SEEK_START);
    bufferAddress = func_002FF538(fileBytes);
    D_003BDA44 = bufferAddress;
    func_0030EF30(fileHandle, bufferAddress, fileBytes);
    func_0030EB78(fileHandle);
    FlushCache(0);
}

extern u32 D_003BDA3C;
extern u32 D_003BDA40;
extern s32 sceSifAllocIopHeap(s32);
extern void func_002F4558(s32, s32, s32);
extern void sdfDevLoadWholeFile(s32);
extern char D_003B4578[];

/* Record the disc search path, preload a separate file, and initialize an aligned IOP buffer. */
void sdfDevStartLoad(s32 discFileNameAddress, s32 preloadFileNameAddress) {
    u32 discFileRecord[SDF_DEV_DISC_FILE_WORDS];
    s32 iopHeapAddress;

    sdfDiscLoadFilename = discFileNameAddress;
    sdfDevLoadWholeFile(preloadFileNameAddress);
    if (sceCdSearchFile(discFileRecord, discFileNameAddress) == 0) {
        sdfPanicHaltPrintf(D_003B4578, discFileNameAddress);
    }
    D_003BDA40 = discFileRecord[0];
    iopHeapAddress = sceSifAllocIopHeap(SDF_DEV_IOP_BUFFER_BYTES);
    D_003BDA3C = iopHeapAddress;
    func_002F4558(0x50, 5, (iopHeapAddress + SDF_DEV_IOP_ALIGNMENT_BIAS) & SDF_DEV_IOP_ALIGNMENT_MASK);
}

void sdfInitDeviceSemaphores(void) {
    sdfDiscSemaphore = sdfCreateSemaphore(1, 0xff, 0);
    sdfDiscRequestSemaphore = sdfCreateSemaphore(0, 0xff, 0);
    sdfDiscRequestPending = 0;
    func_002F3CB8(0);
    func_002F4190(sdfDiscType);
}

extern char sdfDiscPathPrefix[];

/* Convert a relative disc path to the drive's uppercase backslash form with ;1 suffix. */
void sdfDevMakeDiscPath(char *dst, char *src) {
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
extern char *strcpy(char *, char *);
extern char *strcat(char *, char *);

char *sdfDevBuildPath(char *dst, char *src) {
    if (*src == 0x2F) {
        strcpy(dst, D_00398820);
        return strcat(dst, src);
    }
    *(Bytes7 *)dst = D_003BD400[0];
    return strcat(dst, src);
}

void sdfPathPrefixCat(char *destination, char *path) {
    memcpy(destination, sdfPfsPathPrefix, 6);
    strcat(destination, path);
}

u32 func_002E5958(char *path) {
    return 0;
}

u32 func_002E5960(char *path) {
    return 0;
}

u32 func_002E5968(char *path) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5970);

/* Capture seek/size replies, then wake the synchronous command waiter. */
void sdfDevCommandReplyCallback(DevState *state, s32 event, s32 unused, s32 value, s32 callbackContext) {
    if (event != SDF_DEV_EVENT_SEEK_REPLY) {
        if (event == SDF_DEV_EVENT_SIZE_REPLY) {
            sdfDevControlReplyValue = value;
        }
    } else {
        sdfDevOperationReplyValue = value;
    }
    SignalSema(sdfDevReplySemaphore);
}
s32 sdfDevReactivate(DevState *);

/* Open a path using the shared reply callback and wait for its initial completion. */
DevState *sdfDevCreateCommandState(const char *path) {
    DevState *state;
    if (sdfDevReplySemaphore < 0) {
        sdfDevReplySemaphore = sdfCreateSemaphore(0, SDF_DEV_REPLY_SEMAPHORE_LIMIT, 0);
    }
    state = sdfDevCreateCallbackState(path, sdfDevCommandReplyCallback, 0);
    WaitSema(sdfDevReplySemaphore);
    sdfDevReactivate(state);
    return state;
}

s32 sdfPathExists(char *path) {
    char *resolved = func_002E5970(path);
    s32 fd;

    if (*resolved == '/') {
        return sdfPacketExists((u32)resolved);
    }
    fd = func_0030E8F0(resolved, 1);
    func_0030EB78(fd);
    sdfReleaseChipBlock(resolved);
    return fd >= 0;
}

void sdfDevWaitThenReleaseCommandState(DevState *state) {
    sdfDevQueueActiveOperation();
    WaitSema(sdfDevReplySemaphore);
    sdfDevQueueReleaseState(state);
}

/* Queue a byte-counted read, then wait for its reply callback. */
void sdfDevQueueReadAndWait(DevState *state, void *buffer, s32 byteCount) {
    sdfDevQueueRead(state, buffer, byteCount);
    WaitSema(sdfDevReplySemaphore);
}

u32 sdfDevQueueControlAndWait(DevState *state) {
    sdfDevQueueControlRequest(state);
    WaitSema(sdfDevReplySemaphore);
    return sdfDevControlReplyValue;
}

u32 sdfDevQueueOperationAndWait(void) {
    sdfDevQueueOperation();
    WaitSema(sdfDevReplySemaphore);
    return sdfDevOperationReplyValue;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5CD8);

char *sdfDevGetPathBuffer(void) {
    return D_00398820;
}

void func_002E5D80(s32 value) {
    D_003BD3F0 = value;
    func_002E5D98(value);
}

void func_002E5D98(s32 value) {
    D_003BD42E = value;
}

extern DevState *D_003BD418;
extern DevState *D_003BD41C;
extern s16 D_003BD42C;

/* Queue a request on the global list and on its worker's list; wake the worker when its list was empty. */
void sdfDevEnqueueStateAndWakeWorker(DevState *state) {
    DevWorkerEntry *worker;
    s32 interrupts;
    s32 wake = 0;
    DevState *last;

    interrupts = func_00312C08(state);
    worker = &sdfDeviceWorkerEntries[state->workerIndex];
    if (worker->threadId < 0) {
        sdfEnsureDeviceWorkerThreadStarted(state->workerIndex);
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

void sdfDevUnlinkAndFreeState(DevState *state) {
    DevState *prev;
    DevState *next;
    s64 interrupts;

    interrupts = func_00312C08(state);
    prev = state->previous;
    next = state->next;
    if (prev == NULL) {
        D_003BD424 = next;
    } else {
        prev->next = next;
    }
    if (next == NULL) {
        D_003BD428 = prev;
    } else {
        next->previous = prev;
    }
    D_003BD420 -= 1;
    if (interrupts != 0) {
        EIntr();
    }
    sdfReleaseChipBlock(state->resource);
    sdfReleaseChipBlock(state);
}


/* Move state from both active lists to the completed cache, evict the oldest
 * entry at nine cached states, and wake any remaining request on this worker. */
void sdfDevRecycleCompletedState(DevState *state) {
    DevWorkerEntry *worker;
    DevState *prev;
    DevState *next;
    s64 interrupts;

    interrupts = func_00312C08(state);
    prev = state->previous;
    next = state->next;
    if (prev == NULL) {
        D_003BD418 = next;
    } else {
        prev->next = next;
    }
    if (next == NULL) {
        D_003BD41C = prev;
    } else {
        next->previous = prev;
    }
    worker = &sdfDeviceWorkerEntries[state->workerIndex];
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
    state->previous = D_003BD428;
    if (D_003BD428 == NULL) {
        D_003BD424 = state;
    } else {
        D_003BD428->next = state;
    }
    D_003BD428 = state;
    state->next = NULL;
    D_003BD420++;
    D_003BD42C--;
    if (interrupts != 0) {
        EIntr();
    }
    if (D_003BD420 >= SDF_DEV_CACHE_EVICTION_THRESHOLD) {
        sdfDevUnlinkAndFreeState(D_003BD424);
    }
    if (worker->first != NULL) {
        SignalSema(worker->semaphore);
    }
}

/* Close a valid file handle, invalidate it, and retain the completed state in the cache. */
void sdfDevRelease(DevState *state) {
    s32 fileHandle = state->resourceId;

    state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
    if (fileHandle >= 0) {
        func_0030EB78(fileHandle);
    }
    sdfDevRecycleCompletedState(state);
}

/* Store an error/result, release the request, and notify its callback of inactivity. */
void sdfDevDeactivate(DevState *state, s32 result) {
    state->result = result;
    state->state = SDF_DEV_STATE_INACTIVE;
    sdfDevRelease(state);
    if (state->callback != NULL) {
        state->callback(state, SDF_DEV_EVENT_INACTIVE, 0, 0, state->callbackContext);
    }
}

/* Service queued host-file requests for one device worker. */
INCLUDE_RODATA(const s32, "game/code_002E4720", D_003B4578);

void sdfDevWorkerThread(DevWorkerEntry *worker) {
    DevState *state;
    void (*callback)(DevState *, s32, s32, s32, s32);
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
            fileHandle = func_0030E8F0(state->resource, SDF_DEV_FILE_OPEN_READ, 0);
            if (fileHandle < 0) {
                sdfDevDeactivate(state, fileHandle);
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
            fileHandle = func_0030E8F0(state->resource, SDF_DEV_FILE_OPEN_WRITE, state->options);
            if (fileHandle < 0) {
                sdfDevDeactivate(state, fileHandle);
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
            endOffset = func_0030ECF8(state->resourceId, state->operationArg, state->options);
            if (endOffset < 0) {
                sdfDevDeactivate(state, endOffset);
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
            position = func_0030ECF8(state->resourceId, 0, SDF_DEV_SEEK_CURRENT);
            if (position < 0) {
                sdfDevDeactivate(state, position);
                continue;
            }
            endOffset = func_0030ECF8(state->resourceId, 0, SDF_DEV_SEEK_END);
            if (endOffset < 0) {
                sdfDevDeactivate(state, endOffset);
                continue;
            }
            result = func_0030ECF8(state->resourceId, position, SDF_DEV_SEEK_START);
            if (result < 0) {
                sdfDevDeactivate(state, result);
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
            result = func_0030EF30(state->resourceId, (s32)state->requestData,
                           state->requestExtra);
            if (result < 0) {
                sdfDevDeactivate(state, result);
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
            result = func_0030F190(state->resourceId, (s32)state->requestData,
                            state->requestExtra);
            if (result < 0) {
                sdfDevDeactivate(state, result);
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
            result = func_0030EB78(state->resourceId);
            state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
            if (result < 0) {
                sdfDevDeactivate(state, result);
                continue;
            }
            sdfDevRecycleCompletedState(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, SDF_DEV_EVENT_CLOSED, 0, 0, state->callbackContext);
            }
            continue;

        case SDF_DEV_OPERATION_READ_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = func_0030E8F0(state->resource, SDF_DEV_FILE_OPEN_READ, 0);
            if (fileHandle < 0) {
                sdfDevDeactivate(state, fileHandle);
                continue;
            }
            state->resourceId = fileHandle;
            transferResult = state->requestExtra;
            /* A negative count means the whole file; NULL storage is allocated below. */
            if (transferResult < 0) {
                state->state = SDF_DEV_STATE_SEEKING;
                transferResult = func_0030ECF8(fileHandle, 0, SDF_DEV_SEEK_END);
                if (transferResult < 0) {
                    sdfDevDeactivate(state, transferResult);
                    continue;
                }
                result = func_0030ECF8(state->resourceId, 0, SDF_DEV_SEEK_START);
                if (result < 0) {
                    sdfDevDeactivate(state, result);
                    continue;
                }
            }
            if (transferResult != 0) {
                buffer = state->requestData;
                if (buffer == NULL) {
                    buffer = (void *)sdfResourceRetainAddress(sdfAllocGeneralBlock(transferResult));
                }
                state->requestData = buffer;
                state->state = SDF_DEV_STATE_READING;
                transferResult = func_0030EF30(state->resourceId, (s32)buffer, transferResult);
                if (transferResult < 0) {
                    sdfDevDeactivate(state, transferResult);
                    continue;
                }
            }
            state->state = SDF_DEV_STATE_CLOSING;
            func_0030EB78(state->resourceId);
            state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
            sdfDevRecycleCompletedState(state);
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
            fileHandle = func_0030E8F0(state->resource, SDF_DEV_FILE_OPEN_WRITE, state->options);
            if (fileHandle < 0) {
                sdfDevDeactivate(state, fileHandle);
                continue;
            }
            state->resourceId = fileHandle;
            transferResult = state->requestExtra;
            if (transferResult != 0) {
                state->state = SDF_DEV_STATE_WRITING;
                transferResult = func_0030F190(fileHandle, (s32)state->requestData, transferResult);
                if (transferResult < 0) {
                    sdfDevDeactivate(state, transferResult);
                    continue;
                }
            }
            break;

        default:
            continue;
        }

        state->state = SDF_DEV_STATE_CLOSING;
        func_0030EB78(state->resourceId);
        state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
        sdfDevRecycleCompletedState(state);
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
void D_002E6538(DevWorkerEntry *worker) {
    DevState *state;
    void (*callback)(DevState *, s32, s32, s32, s32);
    s32 result;
    s32 transferResult;
    void *data;
    u32 operation;

    for (;;) {
        WaitSema(worker->semaphore);
        state = worker->first;
        if (state == NULL) {
            continue;
        }

        operation = state->operation - 1;
        state->operation = 0;
        switch (operation) {
        case 0:
            state->state = SDF_DEV_STATE_OPENING;
            result = sdfDevOpenDiscFileAndGetSize(state->resource);
            if (result < 0) {
                sdfDevDeactivate(state, 0);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 2, 0, 0, state->callbackContext);
            }
            continue;

        case 3:
            state->state = SDF_DEV_STATE_ACTIVE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 4, 0, sdfDevGetFileSize(),
                                state->callbackContext);
            }
            continue;

        case 4:
            state->state = SDF_DEV_STATE_READING;
            result = func_002E5398(state->requestData, state->requestExtra);
            if (result < 0) {
                sdfDevDeactivate(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 5, (s32)state->requestData, result,
                                state->callbackContext);
            }
            continue;

        case 6:
            state->state = SDF_DEV_STATE_CLOSING;
            sdfServicePendingOperationUnderSemaphore();
            sdfDevRecycleCompletedState(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 7, 0, 0, state->callbackContext);
            }
            continue;

        case 7:
            state->state = SDF_DEV_STATE_OPENING;
            result = sdfDevOpenDiscFileAndGetSize(state->resource);
            if (result < 0) {
                sdfDevDeactivate(state, 0);
                continue;
            }
            transferResult = state->requestExtra;
            if (transferResult < 0) {
                transferResult = sdfDevGetFileSize();
            }
            if (transferResult != 0) {
                data = state->requestData;
                if (data == NULL) {
                    data = (void *)sdfResourceRetainAddress(sdfAllocGeneralBlock(transferResult));
                }
                state->requestData = data;
                state->state = SDF_DEV_STATE_READING;
                transferResult = func_002E5398(data, transferResult);
                if (transferResult < 0) {
                    sdfDevDeactivate(state, transferResult);
                    continue;
                }
            }
            state->state = SDF_DEV_STATE_CLOSING;
            sdfServicePendingOperationUnderSemaphore();
            state->resourceId = -1;
            sdfDevRecycleCompletedState(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 8, (s32)state->requestData, transferResult,
                                state->callbackContext);
            }
            continue;

        case 1:
        case 2:
        case 5:
        case 8:
            sdfDevDeactivate(state, 0);
            continue;

        default:
            continue;
        }
    }
}

extern char D_003BD438[]; /* cdrom0: */
extern char D_003BD440[]; /* host0: */
extern char D_003BD448[]; /* pfs0: */

char *func_002E67A8(char *path, s32 worker) {
    s32 length = strlen(path);
    char *result;
    s32 index;
    s32 character;
    char *base;
    s32 baseLength;
    switch (worker) {
    case 0:
        result = sdfAllocSizeClassBlock(length + 10);
        memcpy(result, D_003BD438, 7);
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
            result = sdfAllocSizeClassBlock(length + 7);
            memcpy(result, D_003BD440, 6);
            memcpy(result + 6, path, length);
            result[length + 6] = 0;
        } else {
            base = sdfDevGetPathBuffer();
            baseLength = strlen(base);
            result = sdfAllocSizeClassBlock(length + baseLength);
            memcpy(result, base, baseLength);
            memcpy(result + baseLength, path + 1, length);
        }
        break;
    case 2:
        result = sdfAllocSizeClassBlock(length + 6);
        memcpy(result, D_003BD448, 5);
        memcpy(result + 5, path, length);
        result[length + 5] = 0;
        break;
    case 3:
        result = sdfAllocSizeClassBlock(length + 1);
        strcpy(result, path);
        break;
    default:
        return NULL;
    }
    return result;
}


INCLUDE_ASM(const s32, "game/code_002E4720", func_002E69F0);

/* Allocate a request state and retain the callback's opaque context word. */
DevState *sdfDevAllocState(void *resource, s32 workerIndex, s32 operation,
                        void (*callback)(DevState *, s32, s32, s32, s32), s32 callbackContext) {
    DevState *state = (DevState *)sdfAllocAndClearQuadwords(SDF_DEV_STATE_BYTES);
    state->resource = resource;
    state->workerIndex = workerIndex;
    state->operation = operation;
    state->state = 0;
    state->resourceId = SDF_DEV_INVALID_FILE_HANDLE;
    state->callback = callback;
    state->callbackContext = callbackContext;
    return state;
}


/* Resolve a path to its worker and enqueue an asynchronous read-only open. */
DevState *sdfDevCreateCallbackState(const char *path, void (*callback)(DevState *, s32, s32, s32, s32),
                        s32 callbackContext) {
    void *resource;
    s32 workerIndex = func_002E69F0(path, &resource);
    DevState *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, workerIndex, SDF_DEV_OPERATION_OPEN_READ, callback, callbackContext);
    sdfDevEnqueueStateAndWakeWorker(state);
    return state;
}

/* Enqueue a write open; zero options select the device's default creation options. */
DevState *sdfDevCreateModeState(const char *path, void (*callback)(DevState *, s32, s32, s32, s32),
                        s32 callbackContext, s32 options) {
    void *resource;
    s32 workerIndex = func_002E69F0(path, &resource);
    DevState *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, workerIndex, SDF_DEV_OPERATION_OPEN_WRITE, callback, callbackContext);
    state->options = options != 0 ? options : sdfDefaultDevRequestOptions;
    sdfDevEnqueueStateAndWakeWorker(state);
    return state;
}

/* Queue a seek with its offset and origin; return -1 unless the request is active. */
s32 sdfDevQueueOperation(DevState *state, s32 seekOffset, s32 seekOrigin) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->operationArg = seekOffset;
    state->options = seekOrigin;
    state->operation = SDF_DEV_OPERATION_SEEK;
    SignalSema(sdfDeviceWorkerEntries[state->workerIndex].semaphore);
    return 0;
}

/* Query file size without changing the file position; return -1 unless active. */
s32 sdfDevQueueControlRequest(DevState *state) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->operation = SDF_DEV_OPERATION_GET_SIZE;
    SignalSema(sdfDeviceWorkerEntries[state->workerIndex].semaphore);
    return 0;
}

/* Queue a read using the supplied data pointer and extra word; reject inactive states with -1. */
s32 sdfDevQueueRead(DevState *state, void *data, s32 extra) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->requestExtra = extra;
    state->operation = 5;
    state->requestData = data;
    SignalSema(sdfDeviceWorkerEntries[state->workerIndex].semaphore);
    return 0;
}


/* Queue a byte-counted write; return -1 without changing an inactive request. */
s32 sdfDevQueueWrite(DevState *state, void *buffer, s32 byteCount) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->requestExtra = byteCount;
    state->operation = SDF_DEV_OPERATION_WRITE;
    state->requestData = buffer;
    SignalSema(sdfDeviceWorkerEntries[state->workerIndex].semaphore);
    return 0;
}


/* Clear an inactive request's result and return it to the active state. */
s32 sdfDevReactivate(DevState *state) {
    if (state->state != SDF_DEV_STATE_INACTIVE) {
        return -1;
    }
    state->result = 0;
    state->state = SDF_DEV_STATE_ACTIVE;
    return 0;
}

/* Active state and close operation share an ID; retain the original byte assignment. */
s32 sdfDevQueueActiveOperation(DevState *request) {
    s8 requestState = request->state;

    if (requestState != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    request->operation = requestState;
    SignalSema(sdfDeviceWorkerEntries[request->workerIndex].semaphore);
    return 0;
}

/* Free a retained active/completed state; reject other states with -1. */
s32 sdfDevQueueReleaseState(DevState *state) {
    if (state->state < SDF_DEV_STATE_INACTIVE) {
        if (state->state >= SDF_DEV_STATE_ACTIVE) {
            sdfDevUnlinkAndFreeState(state);
            return 0;
        }
    }
    return -1;
}


/* Enqueue a one-shot read: negative byte count requests the whole file. */
DevState *sdfDevCreateRequest(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(DevState *, s32, s32, s32, s32), s32 callbackContext) {
    void *resource;
    s32 workerIndex = func_002E69F0(path, &resource);
    DevState *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, workerIndex, SDF_DEV_OPERATION_READ_BUFFER, callback, callbackContext);
    state->requestExtra = byteCount;
    state->requestData = (void *)buffer;
    state->operationArg = 0;
    sdfDevEnqueueStateAndWakeWorker(state);
    return state;
}

/* Enqueue a one-shot write using the supplied buffer, byte count and creation options. */
DevState *sdfDevOpenRequest(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(DevState *, s32, s32, s32, s32),
                        s32 callbackContext, s32 options) {
    void *resource;
    s32 workerIndex = func_002E69F0(path, &resource);
    DevState *state;

    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfDevAllocState(resource, workerIndex, SDF_DEV_OPERATION_WRITE_BUFFER, callback, callbackContext);
    state->requestExtra = byteCount;
    state->requestData = (void *)buffer;
    state->options = options != 0 ? options : sdfDefaultDevRequestOptions;
    state->operationArg = 0;
    sdfDevEnqueueStateAndWakeWorker(state);
    return state;
}

/* Update non-negative worker IDs only; skip an unchanged numeric priority. */
void sdfSetThreadPriorities(s32 priority) {
    DevWorkerEntry *worker;
    u32 index;

    if (sdfDeviceWorkerPriority == priority) {
        return;
    }
    sdfDeviceWorkerPriority = priority;
    worker = sdfDeviceWorkerEntries;
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
void sdfRaiseDeviceThreadPriority(void) {
    sdfDevicePriorityOverrideTicks = SDF_DEV_PRIORITY_OVERRIDE_TICKS;
    sdfSetThreadPriorities(SDF_DEV_OVERRIDE_PRIORITY);
}

/* Restore the default numeric priority without changing the countdown. */
void sdfRestoreDeviceThreadPriority(void) {
    sdfSetThreadPriorities(SDF_DEV_DEFAULT_PRIORITY);
}

/* Decrement an active override and restore the default when it expires. */
void sdfTickThreadPriorityOverride(void) {
    u8 remainingTicks = sdfDevicePriorityOverrideTicks;
    u8 nextTicks;

    if (remainingTicks == 0) {
        return;
    }
    sdfDevicePriorityOverrideTicks = remainingTicks - 1;
    nextTicks = remainingTicks - 1;
    if (nextTicks != 0) {
        return;
    }
    sdfRestoreDeviceThreadPriority();
}

/* Start worker thread `index` if it isn't running; slot 3 runs the alternate entry point. */
void sdfEnsureDeviceWorkerThreadStarted(s32 index) {
    DevWorkerEntry *worker = &sdfDeviceWorkerEntries[index];
    void (*entry)();
    s32 thread;

    thread = worker->threadId;
    if (thread < 0) {
        worker->semaphore = sdfCreateSemaphore(0, 0xFF, 0);
        entry = D_002E6538;
        if (index != 3) {
            entry = sdfDevWorkerThread;
        }
        sdfDeviceWorkerPriority = 0x48;
        thread = sdfCreateThreadWithAllocatedWorkspace(entry, 0x4000, 0x48);
        worker->threadId = thread;
        _StartThread(thread, (s32)worker);
    }
}

void sdfPowerOffLoop(s32 semaphore) {
    s32 status;

    for (;;) {
        WaitSema(semaphore);
        func_003110C8(D_003BD460, 0x5003, 0, 0, 0, 0);
        func_003110C8(D_003BD468, 0x4806, 0, 0, 0, 0);
        sceCdPowerOff(&status);
    }
}

extern s32 iSignalSema(s32);

void sdfPowerOffInterruptCallback(void *semaphore) {
    iSignalSema((s32)semaphore);
}

extern u8 sdfPfsDebugMode;
extern char D_003988C0[];
extern char D_003988D0[];
extern char D_003FA800[];
extern char D_003BD470[];
extern void sdfLoadDevModule(void);
extern s32 sdfCreateThread(void (*)(s32), void *, s32, s32);
typedef void (*SdfPowerOffCallback)(void *);
extern SdfPowerOffCallback func_002F33C8(SdfPowerOffCallback, void *);
extern s32 func_0030E620(void);
extern s32 func_003014F0(char *, const char *, ...);
extern s32 func_00310C00(const char *, const char *, s32, const void *, s32);

void func_002E7228(char *path) {
    char buffer[0x100];
    s32 resident;
    s32 result;
    s32 semaphore;
    s32 thread;

    sdfPfsDebugMode = 0;
    sdfLoadDevModule();
    if (D_003BD478 == 0) {
        return;
    }
    result = func_00312618("cdrom0:\\IRX\\ATAD.IRX;1", 0, NULL, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    result = func_00312618("cdrom0:\\IRX\\HDD.IRX;1", 0xC, D_003988C0, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    result = func_00312618("cdrom0:\\IRX\\PFS.IRX;1", 0x12, D_003988D0, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    semaphore = sdfCreateSemaphore(0, 1, 0);
    thread = sdfCreateThread(sdfPowerOffLoop, D_003FA800, 0x800, 1);
    _StartThread(thread, semaphore);
    func_002F33C8(sdfPowerOffInterruptCallback, (void *)semaphore);
    func_0030E620();
    func_003014F0(buffer, "hdd0:%s,", path);
    func_00310C00(D_003BD470, buffer, 0, 0, 0);
    sdfPfsDebugMode = 1;
}

void sdfLoadDevModule(void) {
    s32 resident;
    if (sdfDevModuleLoaded == 0) {
        D_003BD478 = 0;
        if (func_00312618("cdrom0:\\IRX\\DEV9.IRX;1", 0, NULL, &resident) < 0) {
            func_003003F0("cdrom0:\\IRX\\DEV9.IRX;1 could't load.\n");
            return;
        }
        if (resident != 0) {
            func_003003F0(D_003B4690);
            return;
        }
        D_003BD478 = 1;
        sdfDevModuleLoaded = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E73F0);

INCLUDE_ASM(const s32, "game/code_002E4720", sdfClearQuadwords);

char *sdfStrDup(const char *text) {
    u32 length;
    char *copy;

    if (text == NULL) {
        return NULL;
    }
    length = strlen(text);
    copy = sdfAllocSizeClassBlock(length + 1);
    memcpy(copy, text, length);
    copy[length] = 0;
    return copy;
}

s32 sdfBcdStrToInt(s32 packedDigits) {
    s32 place = 1;
    s32 acc = 0;

    while (packedDigits > 0) {
        acc += (packedDigits & 0xf) * place;
        packedDigits >>= 4;
        place *= 10;
    }
    return acc;
}

/* Pack positive decimal digits into nibbles; non-positive input returns zero.
 * The original signed shifts and lack of an overflow check are retained.
 */
s32 sdfDecimalToPackedDigits(s32 number) {
    s32 digitShift = 0;
    s32 packedDigits = 0;

    while (number > 0) {
        s32 quotient = number / SDF_DECIMAL_RADIX;
        packedDigits |= (number - quotient * SDF_DECIMAL_RADIX) << digitShift;
        number = quotient;
        digitShift += SDF_BCD_DIGIT_BITS;
    }
    return packedDigits;
}

/* Allocate element storage with a fixed element stride and growth increment. */
DevRequest *sdfDevCreateBufferedRequest(s32 elementCount, s32 elementStride, s32 growthStep) {
    DevRequest *request = sdfAllocSizeClassBlock(sizeof(*request));

    request->growStep = growthStep;
    request->usedCount = 0;
    request->capacity = elementCount;
    request->stride = elementStride;
    if (elementCount != 0) {
        request->handle = sdfAllocGeneralBlock(elementStride * elementCount);
        request->buffer = (void *)sdfResourceRetainAddress(request->handle);
    } else {
        request->handle = 0;
        request->buffer = 0;
    }
    return request;
}

/* Release the backing allocation and the request object. */
void sdfDestroyDevRequest(DevRequest *request) {
    sdfReleaseResourceAllocation(request->handle);
    sdfReleaseChipBlock(request);
}

void sdfDevResizeBufferedRequest(DevRequest *request, s32 count);

/* Grow capacity, preserving the SDK's signed 16-bit allocation-size arithmetic. */
void sdfDevBufferedRequestGrow(DevRequest *request) {
    if (request->handle == 0) {
        sdfDevResizeBufferedRequest(request, request->growStep);
        return;
    }
    sdfDecrementAllocationReferenceCount(request->handle);
    request->capacity = request->capacity + request->growStep;
    func_002D0750(request->handle, (s16)request->capacity * request->stride);
    request->buffer = (void *)sdfResourceRetainAddress(request->handle);
}

/* Resize storage and clamp the live entry count to the new capacity. */
void sdfDevResizeBufferedRequest(DevRequest *request, s32 elementCount) {
    if (request->handle == 0) {
        if (elementCount > 0) {
            request->capacity = elementCount;
            request->handle = sdfAllocGeneralBlock(request->stride * elementCount);
            request->buffer = (void *)sdfResourceRetainAddress(request->handle);
        }
    } else if (elementCount <= 0) {
        sdfReleaseResourceAllocation(request->handle);
        request->handle = 0;
        request->usedCount = 0;
        request->capacity = 0;
        request->buffer = 0;
    } else {
        sdfDecrementAllocationReferenceCount(request->handle);
        request->capacity = elementCount;
        func_002D0750(request->handle, request->stride * elementCount);
        request->buffer = (void *)sdfResourceRetainAddress(request->handle);
        if (elementCount < request->usedCount) {
            request->usedCount = elementCount;
        }
    }
}


/* Mirror the phase into a quarter turn, then evaluate an odd ninth-degree polynomial. */
f32 sdfSinPoly(f32 angle) {
    f32 phase = angle * SDF_TRIG_INVERSE_TAU;
    f32 polynomialInput;
    f32 inputSquared;
    f32 inputCubed;
    f32 inputFifthPower;
    f32 inputSeventhPower;
    f32 inputNinthPower;

    phase -= (s32)phase;
    if (phase > 0.5f) {
        phase -= 1.0f;
    } else if (phase < -0.5f) {
        phase += 1.0f;
    }
    if (phase > 0.25f) {
        phase = 0.5f - phase;
    } else if (phase < -0.25f) {
        phase = -0.5f - phase;
    }
    polynomialInput = phase * SDF_SINE_POLYNOMIAL_SCALE;
    inputSquared = polynomialInput * polynomialInput;
    inputCubed = inputSquared * polynomialInput;
    inputFifthPower = inputCubed * inputSquared;
    inputSeventhPower = inputFifthPower * inputSquared;
    inputNinthPower = inputSeventhPower * inputSquared;
    return polynomialInput * SDF_TRIG_HALF_PI + inputCubed * -0.64596367f + inputFifthPower * 0.07968968f + inputSeventhPower * -0.0046737656f + inputNinthPower * 0.00015148419f;
}


/* Apply a quarter-turn phase shift to the existing sine approximation. */
f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle) {
    return sdfSinPoly(angle + SDF_TRIG_HALF_PI);
}

/* Binary-search samples, using zero as the implicit lower endpoint.
 * The step is integer 1/sampleCount converted to f32, not a float reciprocal.
 * The caller must provide valid table bounds for the final sample index.
 */
f32 sdfTableInterpolate(f32 value, f32 *table, s32 sampleCount) {
    f32 positionStep = 1 / sampleCount;
    s32 lowerIndex = 0;
    s32 upperIndex = sampleCount;
    s32 sampleIndex;
    f32 lowerValue;
    f32 upperValue;

    do {
        sampleIndex = lowerIndex + upperIndex;
        sampleIndex >>= 1;
        upperValue = table[sampleIndex];
        if (value < upperValue) {
            upperIndex = sampleIndex;
        } else {
            sampleIndex++;
            lowerIndex = sampleIndex;
        }
    } while (lowerIndex < upperIndex);
    upperValue = table[sampleIndex];
    lowerValue = 0.0f;
    if (sampleIndex != 0) {
        lowerValue = table[sampleIndex - 1];
    }
    return sampleIndex * positionStep + (value - lowerValue) * positionStep / (upperValue - lowerValue);
}

/* Odd fifth-degree atan approximation; the ratio is not range-checked here. */
f32 sdfAtan2Poly(f32 ratio) {
    f32 ratioSquared = ratio * ratio;
    f32 ratioCubed = ratioSquared * ratio;
    f32 ratioFifthPower = ratioSquared * ratioCubed;

    return ratio * 0.99999977f + ratioCubed * -0.33325735f + ratioFifthPower * 0.19388643f;
}

f32 sdfAtan2(f32 y, f32 x) {
    s32 sx = 0;
    s32 sy;
    f32 r;

    if (x < 0.0f) {
        x = -x;
        sx = 1;
    }
    sy = 0;
    if (y < 0.0f) {
        y = -y;
        sy = 1;
    }
    if (y < x) {
        r = sdfAtan2Poly(y / x);
    } else {
        r = 1.5707963f - sdfAtan2Poly(x / y);
    }
    if (sx != 0) {
        r = 3.1415926f - r;
    }
    if (sy != 0) {
        r = -r;
    }
    return r;
}

/* Restore the input sign after sampling; magnitudes at or above one use half pi. */
f32 sdfAsinTable(f32 x) {
    f32 inputSign;
    f32 angleMagnitude;

    if (x < 0.0f) {
        x = -x;
        inputSign = -1.0f;
    } else {
        inputSign = 1.0f;
    }
    angleMagnitude = x >= 1.0f ? SDF_TRIG_HALF_PI : sdfTableInterpolate(x, sdfNormalizedAsinSamples, SDF_ASIN_SAMPLE_COUNT) * SDF_TRIG_HALF_PI;
    return angleMagnitude * inputSign;
}

/* Return the input-signed complement of the sampled angle, not standard acos(x).
 * Magnitudes at or above one leave the angle magnitude at zero.
 */
f32 sdfAcosTable(f32 x) {
    f32 inputSign;
    f32 angleMagnitude;

    if (x < 0.0f) {
        x = -x;
        inputSign = -1.0f;
    } else {
        inputSign = 1.0f;
    }
    angleMagnitude = 0.0f;
    if (!(x >= 1.0f)) {
        angleMagnitude = (1.0f - sdfTableInterpolate(x, sdfNormalizedAsinSamples, SDF_ASIN_SAMPLE_COUNT)) * SDF_TRIG_HALF_PI;
    }
    return angleMagnitude * inputSign;
}

/* Keep the cast-plus/minus-one turn adjustment; this is not general modulo.
 * Inputs already within the pi thresholds are returned unchanged.
 */
f32 sdfWrapAngle(f32 angle) {
    s32 adjustedTurns;
    if (angle > SDF_TRIG_PI) {
        adjustedTurns = (s32)(angle / SDF_TRIG_TAU) + 1;
        return angle - (f32)adjustedTurns * SDF_TRIG_TAU;
    }
    if (angle < -SDF_TRIG_PI) {
        adjustedTurns = (s32)(angle / SDF_TRIG_TAU) - 1;
        return angle - (f32)adjustedTurns * SDF_TRIG_TAU;
    }
    return angle;
}

INCLUDE_RODATA(const s32, "game/code_002E4720", D_003B4690);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3C8);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3D0);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDiscRequestPending);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3E0);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDiscType);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD3F0);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDevReplySemaphore);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDiscPathPrefix);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD400);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfPfsPathPrefix);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD410);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD418);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD41C);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD420);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD424);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD428);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD42C);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD42E);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDevicePriorityOverrideTicks);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDeviceWorkerPriority);

INCLUDE_SDATA(const s32, "game/code_002E4720", sdfDefaultDevRequestOptions);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD438);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD440);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD448);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD450);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD458);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD460);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD468);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD470);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD478);

INCLUDE_SDATA(const s32, "game/code_002E4720", D_003BD480);

