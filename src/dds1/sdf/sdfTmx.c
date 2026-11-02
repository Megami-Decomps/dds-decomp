#include "common.h"
#include "ps2asm.h"
#include "sdf_resource.h"
#define SDK_SKIP_SleepThread
#define SDK_SKIP_sceGetstat
#define SDK_SKIP_sceSifSetRpcQueue
#define SDK_SKIP_sceSifRpcLoop
typedef void *(*sceSifRpcFunc)(s32 function, void *request, s32 size);
#include "sif_rpc.h"

enum {
    SDF_TMX_MAGIC = 0x30584D54,
    SDF_STREAM_CHUNK_BYTES = 0x4000,
    SDF_STREAM_COMMAND_CREATE_STATE = 32,
    SDF_STREAM_COMMAND_CONTROL = 33,
    SDF_STREAM_COMMAND_READ = 34,
    SDF_STREAM_COMMAND_RELEASE_STATE = 35,
    SDF_IOP_MODULES_PER_GROUP = 2,
    SDF_KEY_TREE_PATH_LIMIT = 32,
    SDF_POOL_REBUILD_CHAIN = 0,
    SDF_POOL_PUSH_FREE = 1
};

struct TMX_HEADER_t;

struct sdfFileCmdHandle_t;

struct sdfFileRpcReadReq_t;

struct sdfFileRpcCb_t;

/* Read the raw free-running count from EE timer 0. */
extern u32 sdfGetRunTimeCountBase(void);

/* Return the modulo-16-bit elapsed count from the EE timer 0 value. */
extern u32 sdfGetRunTimeCount(u32 aBaseTime);

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

/* Protocol IDs use distinct byte-sized storage codes in DevState. */
typedef u8 SdfDevOperationCode;

typedef s8 SdfDevStateCode;

/* The reason code sdfFileCmdHandle_t.callback is invoked with. */
enum SdfFileIOCmdClbk {
    SDF_FILE_CMD_CLBK_ERROR = 0,
    SDF_FILE_CMD_CLBK_CANCEL = 1,
    SDF_FILE_CMD_CLBK_OPEN = 2,
    SDF_FILE_CMD_CLBK_SEEK = 3,
    SDF_FILE_CMD_CLBK_FILESIZE = 4,
    SDF_FILE_CMD_CLBK_READ = 5,
    SDF_FILE_CMD_CLBK_WRITE = 6,
    SDF_FILE_CMD_CLBK_CLOSE = 7,
    SDF_FILE_CMD_CLBK_DONE = 8,
    SDF_FILE_CMD_CLBK_FREE = 9
};

#define D_00594940 D_003FF4C0
#define D_00594800 D_003FF380

extern u8 D_00594940[];

extern void SleepThread(void);

extern char *mcModuleNames[];

extern s32 sceSifLoadModule(const char *, s32, const char *);

extern char *strcpy(char *, const char *);

extern char *strcat(char *, const char *);

extern void sceMcInit(void);

/* Allocate a workspace sized by stackBytes, pass the thread entry to
 * CreateThreadSimple, and return that function's result unchanged. */
s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);


/* Complete 0x40-byte owner allocated by sdfFileCmdNewHandle. */
typedef struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next; /* 0x00 */
    struct sdfFileCmdHandle_t *prev; /* 0x04 */
    struct sdfFileCmdHandle_t *deviceNext; /* 0x08 */
    struct sdfFileCmdHandle_t *devicePrev; /* 0x0C */
    char *filename; /* 0x10 */
    u8 device; /* 0x14 */
    SdfDevOperationCode cmd; /* 0x15 */
    SdfDevStateCode stat; /* 0x16 */
    u8 phase; /* 0x17 */
    s32 offset; /* 0x18 */
    s32 length; /* 0x1C */
    void *buffer; /* 0x20 */
    s32 cmdParm; /* 0x24 */
    s32 fd; /* 0x28 */
    s32 errorCode; /* 0x2C */
    s32 currentOffset; /* 0x30 */
    u8 remainder[4]; /* 0x34 */
    void (*callback)(struct sdfFileCmdHandle_t *, s32, s32, s32, s32); /* 0x38 */
    s32 userData; /* 0x3C: opaque callback-context word */
} sdfFileCmdHandle_t;

s32 sdfFileCmdRead(sdfFileCmdHandle_t *aHandle, void *aBuffer, s32 aLength);

s32 sdfFileCmdFree(sdfFileCmdHandle_t *aHandle);

s32 sdfFileCmdClearError(sdfFileCmdHandle_t *aHandle);

s32 sdfFileCmdClose(sdfFileCmdHandle_t *aHandle);

typedef struct sdfFileRpcReadReq_t {
    u8 opaque00[4];
    s32 destinationAddress; /* Caller-provided address word in the read request. */
    s32 byteCount;
} sdfFileRpcReadReq_t;

typedef struct sdfFileRpcCb_t {
    sdfFileCmdHandle_t *deviceState;
    s32 semaphore;
    s32 readResult;
    s32 remainingBytes;
    u8 *destination; /* +0x10: current DMA destination; advanced per completed chunk. */
    s32 transferredBytes;
} sdfFileRpcCb_t;

extern sdfFileRpcCb_t sdfFileRpcState;

extern s32 sdfFileRpcBuf[];

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path,
                                          s32 (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                                          s32 callbackContext);

extern s32 sdfFileCmdFileSize();

extern s32 WaitSema(s32);

extern void FlushCache(s32);

extern s32 SignalSema(s32);

#include "sdk_libkernl.h"

extern s32 sceSifDmaStat(s32);

extern s32 GetThreadId(void);

extern s32 sdfFileRpcThreadId;

void SdfFileRpcServerThreadProc(void);

void sdfTmxSetHeader(TMX_HEADER_t *aHeader, s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfCLUT, s32 aCLUTFormat);

void sdfFileRpcContinueRead(void);

s32 sdfFileRpcReadCallback(sdfFileCmdHandle_t *deviceState, s32 command, s32 sourceAddress,
                  s32 byteCount, s32 callbackArg);

void *sdfFileRpcHandler(s32 function, void *request, s32 requestSize);

void sdfStartFileRpcServer(void);

void LoadMcModules(const char *prefix, s32 moduleGroup);

void sdfTmxSetHeader(TMX_HEADER_t *aHeader, s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfCLUT, s32 aCLUTFormat) {
    memset(aHeader, 0, sizeof(TMX_HEADER_t));
    aHeader->CLUTFormat = aCLUTFormat;
    aHeader->width = aWidth;
    aHeader->height = aHeight;
    aHeader->pixelFormat = aPixelFormat;
    aHeader->numOfCLUT = aNumOfCLUT;
    aHeader->formatId = SDF_TMX_MAGIC;
    aHeader->fileType = 2;
}

void sdfFileRpcContinueRead(void) {
    s32 chunkBytes = sdfFileRpcState.remainingBytes;

    if (chunkBytes > SDF_STREAM_CHUNK_BYTES) {
        chunkBytes = SDF_STREAM_CHUNK_BYTES;
    }
    sdfFileCmdRead(sdfFileRpcState.deviceState, D_00594940, chunkBytes);
}

s32 sdfFileRpcReadCallback(sdfFileCmdHandle_t *deviceState, s32 command, s32 sourceAddress,
                  s32 byteCount, s32 callbackArg) {
    sdfFileRpcCb_t *stream = &sdfFileRpcState;
    s32 dmaId;
    s32 timer;

    switch (command) {
    case SDF_FILE_CMD_CLBK_FILESIZE:
        stream->readResult = byteCount;
        SignalSema(stream->semaphore);
        break;
    case SDF_FILE_CMD_CLBK_READ: {
        struct {
            s32 source;
            s32 destination;
            s32 size;
            s32 attributes;
        } transfer;

        FlushCache(0);
        transfer.source = sourceAddress;
        transfer.destination = (s32)stream->destination;
        transfer.size = byteCount;
        transfer.attributes = 0;
        dmaId = sceSifSetDma(&transfer, 1);
        while (sceSifDmaStat(dmaId) >= 0) {
            timer = sdfGetRunTimeCountBase();
            while ((s32)sdfGetRunTimeCount(timer) <= 0) {
            }
        }
        timer = sdfGetRunTimeCountBase();
        while ((s32)sdfGetRunTimeCount(timer) < 0x64) {
        }
        stream->destination += byteCount;
        stream->transferredBytes += byteCount;
        stream->remainingBytes -= byteCount;
        if (stream->remainingBytes > 0) {
            sdfFileRpcContinueRead();
        } else {
            SignalSema(stream->semaphore);
        }
        break;
    }
    case SDF_FILE_CMD_CLBK_OPEN:
    case SDF_FILE_CMD_CLBK_CLOSE:
        SignalSema(stream->semaphore);
        break;
    case SDF_FILE_CMD_CLBK_ERROR:
        SignalSema(stream->semaphore);
        break;
    }
    return 0;
}

void *sdfFileRpcHandler(s32 function, void *request, s32 requestSize) {
    sdfFileRpcCb_t *state = &sdfFileRpcState;
    sdfFileCmdHandle_t *deviceState;
    u32 command = (u32)function;

    switch (command) {
    case SDF_STREAM_COMMAND_CREATE_STATE:
        state->deviceState = sdfFileCmdReadOpen((const char *)request, sdfFileRpcReadCallback, 0);
        if (state->deviceState != 0) {
            WaitSema(state->semaphore);
            deviceState = state->deviceState;
            if (deviceState->errorCode != 0) {
                sdfFileCmdClearError(deviceState);
                sdfFileCmdFree(state->deviceState);
                state->deviceState = 0;
                deviceState = 0;
            }
        } else {
            deviceState = 0;
        }
        sdfFileRpcBuf[0] = (s32)deviceState;
        break;
    case SDF_STREAM_COMMAND_CONTROL:
        sdfFileCmdFileSize(state->deviceState, (s32)request, (s32)request);
        WaitSema(state->semaphore);
        sdfFileRpcBuf[0] = state->readResult;
        break;
    case SDF_STREAM_COMMAND_READ:
        state->remainingBytes = ((sdfFileRpcReadReq_t *)request)->byteCount;
        state->transferredBytes = 0;
        state->destination = (u8 *)((sdfFileRpcReadReq_t *)request)->destinationAddress;
        sdfFileRpcContinueRead();
        WaitSema(state->semaphore);
        sdfFileRpcBuf[0] = state->transferredBytes;
        break;
    case SDF_STREAM_COMMAND_RELEASE_STATE:
        sdfFileCmdClose(state->deviceState);
        WaitSema(state->semaphore);
        sdfFileCmdFree(state->deviceState);
        state->deviceState = 0;
        sdfFileRpcBuf[0] = 0;
        break;
    }
    return sdfFileRpcBuf;
}

typedef struct _reent _reent;

typedef __builtin_va_list va_list;

typedef struct __sbuf {
    u8 *_base;
    s32 _size;
} __sbuf;

typedef struct __sFILE {
    u8 *_p;
    s32 _r;
    s32 _w;
    s16 _flags;
    s16 _file;
    __sbuf _bf;
    s32 _lbfsize;
    void *_cookie;
    s32 (*_read)(void *cookie, char *buf, s32 n);
    s32 (*_write)(void *cookie, const char *buf, s32 n);
    long (*_seek)(void *cookie, s32 offset, s32 whence);
    s32 (*_close)(void *cookie);
    __sbuf _ub;
    u8 *_up;
    s32 _ur;
    u8 _ubuf[3];
    u8 _nbuf[1];
    __sbuf _lb;
    s32 _blksize;
    s32 _offset;
    _reent *_data;
} FILE;

typedef struct __siov {
    void *iov_base;
    u32 iov_len;
} __siov;

typedef struct __suio {
    __siov *uio_iov;
    s32 uio_iovcnt;
    s32 uio_resid;
} __suio;

typedef struct _Bigint {
    struct _Bigint *next;
    s32 k;
    s32 maxwds;
    s32 sign;
    s32 wds;
    u32 x[1];
} _Bigint;

typedef struct _glue {
    struct _glue *next;
    s32 niobs;
    FILE *iobs;
} _glue;

/* libgcc frame-dwarf2.c's object list node. __register_frame_info stores the
 * FDE base and clears the rest; __register_frame allocates one of these. */
typedef struct object {
    void *pc_begin;
    void *pc_end;
    void *fde_begin;
    void *fde_array;
    void *count;
    struct object *next;
} object;

/* libgcc's fde_vector, as add_fdes fills it. */
typedef struct fde_vector {
    u32 **array;
    s32 count;
} fde_vector;

typedef struct _atexit {
    struct _atexit *_next;
    s32 _ind;
    void (*_fns[32])(void);
} _atexit;

struct _reent {
    s32 _errno;
    FILE *_stdin;
    FILE *_stdout;
    FILE *_stderr;
    s32 _inc;
    char _emergency[25];
    s32 _current_category;
    const char *_current_locale;
    s32 __sdidinit;
    void (*__cleanup)(struct _reent *r);
    _Bigint *_result;
    s32 _result_k;
    _Bigint *_p5s;
    _Bigint **_freelist;
    u8 _unused[0xF8];
    _atexit *_atexit;
    _atexit _atexit0;
    void (**_sig_func)(s32 sig);
    struct _glue __sglue;
    FILE __sf[3];
};

extern _reent *_impure_ptr __attribute__((section(".data")));

extern s32 _puts_r(_reent *r, const char *s);

extern s32 _raise_r(_reent *r, s32 sig);

extern void *_signal_r(_reent *r, s32 sig, void *handler);

extern s32 _setlocale_r(_reent *r, s32 category, const char *locale);

extern void *func_002DA308(_reent *r);

extern void _cleanup_r(_reent *r);

extern s32 fflush(void *fp);

extern s32 _fwalk(_reent *r, s32 (*func)(void *));

extern s32 libc_2DC670(void);

extern long _strtol_r(_reent *r, const char *s, char **end, s32 base);

extern long strtol(const char *s, char **end, s32 base);

extern s32 raise(s32 sig);

extern void func_001001C8(s32 code) __attribute__((noreturn));

extern u8 D_0054B8D8[] __attribute__((section(".data")));

extern s32 D_00552178 __attribute__((section(".data")));

extern s32 D_0052C28C __attribute__((section(".data")));

extern s32 D_0052C288 __attribute__((section(".data")));

extern s32 D_0052C378 __attribute__((section(".data")));

extern s32 _init_signal_r(_reent *r);

extern void gcc2_compiled__2(FILE *fp, s32 flags, s32 file, _reent *data);

extern _glue *__sfmoreglue(_reent *r, s32 n);

extern s32 __sigtramp_r(_reent *r, s32 sig);

extern void *__deregister_frame_info(void *fn);

extern void free(void *ptr);

extern void *malloc(u32 size);

extern void __malloc_lock(_reent *r);

extern void __malloc_unlock(_reent *r);

extern void *_malloc_r(_reent *r, u32 size);

extern void _free_r(_reent *r, void *ptr);

extern s32 SignalSema(s32 sema);

extern void *sbrk(s32 incr);

extern s32 close(s32 fd);

extern _Bigint *_Balloc(_reent *r, s32 k);

extern s32 __register_frame_info(void *fn, void *entry);

extern s32 __register_frame_info_table(void *fn, void *entry);

extern s32 write(s32 fd, const void *buf, s32 count);

extern s32 read(s32 fd, void *buf, s32 count);

extern long lseek(s32 fd, s32 offset, s32 whence);

extern s32 fstat(s32 fd, void *buf);

extern s32 kill(s32 pid, s32 sig);

extern long _read_r(_reent *r, s32 fd, char *buf, s32 n);

extern long _write_r(_reent *r, s32 fd, const char *buf, s32 n);

extern long _lseek_r(_reent *r, s32 fd, s32 offset, s32 whence);

extern s32 __sfvwrite(FILE *fp, __suio *uio);

extern s32 __sread(void *cookie, char *buf, s32 n);

extern s32 __swrite(void *cookie, const char *buf, s32 n);

extern long __sseek(void *cookie, s32 offset, s32 whence);

extern s32 __sclose(void *cookie);

extern s32 GetThreadId(void);

extern s32 WaitSema(s32 sema);

extern s32 vfprintf(FILE *fp, const char *fmt, va_list ap);

extern void *memset(void *a, s32 b, u32 c);

extern void *_calloc_r(_reent *r, u32 n, u32 size);

extern const double D_0054B930[];

extern void abort(void);

extern s32 __do_global_ctors(void);

extern s32 atexit(void (*fn)(void));

extern void (**D_0052BB28)(void) __attribute__((section(".data")));

extern s32 D_0052BB2C __attribute__((section(".data")));

extern s32 D_0052C37C __attribute__((section(".data")));

extern u8 D_0052D940[] __attribute__((section(".data")));

extern void (*D_002F5600[])(void) __attribute__((section(".data")));

extern s32 D_0059AD58 __attribute__((section(".data")));

extern object *D_0059AD60 __attribute__((section(".data")));

extern object D_0059AD40;

typedef struct fp_number_type {
    s32 class;
    s32 sign;
    s32 normal_exp;
    u32 fraction;
} fp_number_type;

extern void sdfUnpackFloat(const void *x, fp_number_type *u);

extern f32 sdfPackFloat(const fp_number_type *u);

extern long __make_dp(s32 kind, s32 sign, s32 exp, long mant);

#define va_start(ap, last) __builtin_stdarg_start((ap), last)

#define va_end(ap) ((void)0)

extern s32 func_002D6678(_reent *r, FILE *fp, const char *fmt, va_list ap);

typedef union f32_bits {
    f32 f;
    u32 u;
} f32_bits;

/* The same record from the double-precision build of fp-bit.c. */
typedef struct fp_number_type_df {
    s32 class;
    s32 sign;
    s32 normal_exp;
    long fraction;
} fp_number_type_df;

extern long __pack_df(const fp_number_type_df *u);

extern void sdfUnpackDouble(const double *x, fp_number_type_df *u);

extern fp_number_type_df *libc_2D1590(const fp_number_type_df *a, const fp_number_type_df *b, fp_number_type_df *r);

extern long __fpcmp_parts_df(const fp_number_type_df *a, const fp_number_type_df *b);

extern const u8 D_0054B551[];

#define SDF_TOLOWER(c) ((D_0054B551[(c)] & 1) ? (c) + 0x20 : (c))

typedef struct {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} sdfSceIoStat_t;

typedef char sdfSceIoStat_size_must_be_0x40[
    sizeof(sdfSceIoStat_t) == 0x40 ? 1 : -1];

extern s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);
extern s32 sceGetstat(const char *path, sdfSceIoStat_t *stat);
extern s32 SdfIopFileRpc(const char *name, s32 dataSize, const void *data, s32 *outSize);
extern s32 WakeupThread(s32 threadId);
extern u8 sdfHddValid;
/* DDS compiles the two module paths from string literals. */
#define D_0054A360 "cdrom0:\\USERIRX\\SDFIOPFL.IRX;1"
#define D_0054A380 "pfs0:/userirx/sdfiopfl.irx"
extern u8 D_00594800[];

void SdfFileRpcServerThreadProc(void)
{
    sdfSceIoStat_t stat;
    sifRpcDataQueue_t queue;
    sifRpcServerData_t server;
    const char *modulePath;
    sdfFileRpcCb_t *state = &sdfFileRpcState;

    state->semaphore = sdfCreateSema(0, 1, 0);
    sceSifSetRpcQueue(&queue, GetThreadId());
    sceSifRegisterRpc((struct sifRpcServerData_t *)&server, 0x66504F49, (sceSifRpcFunc)sdfFileRpcHandler, D_00594800, NULL, NULL, (struct sifRpcDataQueue_t *)&queue);
    modulePath = D_0054A360;
    if (sdfHddValid != 0) {
        if (sceGetstat(D_0054A380, &stat) == 0) {
            if ((stat.mode & 0xF000) == 0x2000) {
                modulePath = D_0054A380;
            }
        }
    }
    SdfIopFileRpc(modulePath, 0, NULL, NULL);
    WakeupThread(sdfFileRpcThreadId);
    sceSifRpcLoop(&queue);
}


void sdfStartFileRpcServer(void) {
    s32 workerId = sdfCreateThreadWithStack(SdfFileRpcServerThreadProc, 0x1000, 0x4C);

    _StartThread(workerId, 0);
    sdfFileRpcThreadId = GetThreadId();
    SleepThread();
}

void LoadMcModules(const char *prefix, s32 moduleGroup) {
    char path[0x100];
    char **moduleNames = &mcModuleNames[moduleGroup * SDF_IOP_MODULES_PER_GROUP];
    s32 moduleIndex;

    for (moduleIndex = 0; moduleIndex != SDF_IOP_MODULES_PER_GROUP; moduleIndex++) {
        strcpy(path, prefix);
        strcat(path, moduleNames[moduleIndex]);
        sceSifLoadModule(path, 0, 0);
    }
    sceMcInit();
}

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_003B50C0);

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_003B50D0);

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_003B50E0);

INCLUDE_RODATA(const s32, "sdf/sdfTmx", D_003B50F0);
