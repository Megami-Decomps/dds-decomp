#define SDK_SKIP_strlen
#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

s32 strlen(const char *aStr);

typedef struct sifRpcClientDataPrefix_t {
    u8 opaque00[0x24];
    void *server;
} sifRpcClientDataPrefix_t;

typedef struct sifRpcClientContextView_t {
    sifRpcClientDataPrefix_t client;
    u8 opaque28[8];
} sifRpcClientContextView_t;

typedef void (*sifMRpcEnd_t)(void *argument);

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

#include "sdk_libmrpc.h"

extern sifRpcClientContextView_t sdfPrintRpcClient;

extern s8 sdfPrintIopFlag;

extern char sdfPrintStrFmt[];

extern s32 scePrintf(const char *, ...);

extern s32 vsprintf(char *destination, const char *format, void *args);

extern s32 (*p2sdSmgBuf)(void);

/* Copy a positive source/end span into the RPC reply buffer; otherwise return NULL. */
void *sdfPrintRpcFunc(s32 unused, sifCommand_t *packet) {
    s32 copyBytes = packet->end - packet->source;
    if (copyBytes > 0) {
        memcpy(packet, EE_ADDR_AS(void *, packet->source), copyBytes);
        return packet;
    }
    return NULL;
}

void sdfPrintRpcThread();

typedef struct sifPrintRpcServerData_t sifPrintRpcServerData_t;
typedef struct sifRpcDataQueue_t {
    s32 threadId;
    s32 active;
    sifPrintRpcServerData_t *serverList;
    sifPrintRpcServerData_t *request;
    s32 requestState;
    struct sifRpcDataQueue_t *next;
} __attribute__((aligned(16))) sifRpcDataQueue_t;

struct sifPrintRpcServerData_t {
    s32 serviceId;
    void *(*handler)(s32, sifCommand_t *);
    void *buffer;
    u32 opaque0C;
    s32 bufferSize;
    void *callback;
    u8 opaque18[0x20];
    sifPrintRpcServerData_t *next;
    sifPrintRpcServerData_t *nextRequest;
    sifRpcDataQueue_t *queue;
};

SIZE_CHECK(sifRpcDataQueue_size_must_be_0x20, sifRpcDataQueue_t, 0x20);
SIZE_CHECK(sifPrintRpcServerData_size_must_be_0x44, sifPrintRpcServerData_t, 0x44);

extern void *sdfPrintRpcFunc(s32 unused, sifCommand_t *packet);
extern s32 GetThreadId(void);
extern s32 sceSifSetRpcQueue(sifRpcDataQueue_t *queue, s32 threadId);
#include "sdk_libkernl.h"

extern void sceSifRpcLoop(sifRpcDataQueue_t *queue);
extern u8 sdfPrintRpcBuff[];

void sdfPrintRpcThread(void)
{
    sifRpcDataQueue_t queue;
    sifPrintRpcServerData_t server;

    sceSifSetRpcQueue(&queue, GetThreadId());
    sceSifRegisterRpc((struct sifRpcServerData_t *)&server, 0x32647270, (sceSifRpcFunc)sdfPrintRpcFunc, sdfPrintRpcBuff, 0, NULL, (struct sifRpcDataQueue_t *)&queue);
    sceSifRpcLoop(&queue);
}


extern char sdfPrintIopBindErrMsg[];

#include "sdk_libc.h"

extern s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);

extern s32 sdfGetRunTimeCountBase(void);

extern s32 sdfGetRunTimeCount(s32 aBaseTime);

extern s32 sceSifMBindRpc(sifRpcClientDataPrefix_t *client, s32 number, s32 mode);

/* DDS retries the bind after a failure instead of printing and returning. */
void sdfPrintIopInit(void)
{
    s32 startTicks;

    _StartThread(sdfCreateThreadWithStack(sdfPrintRpcThread, 0x1000, 0x4C), 0);
    while (sceSifMBindRpc(&sdfPrintRpcClient.client, 0x646E7270, 0) >= 0) {
        if (sdfPrintRpcClient.client.server != NULL) {
            sdfPrintIopFlag = 1;
            break;
        }
        startTicks = sdfGetRunTimeCountBase();
        while (sdfGetRunTimeCount(startTicks) < 4) {
        }
    }
}

/* Print text through the bound IOP dev client, or locally when it is unbound. */
void sdfPuts(const char *str) {
    u8 buffer[0x13F];
    u8 *alignedBuffer;
    s32 length;
    if (sdfPrintIopFlag != 0) {
        length = strlen(str);
        if (length != 0) {
            alignedBuffer = EE_ADDR_AS(u8 *, ((EE_ADDR(&buffer[0x3F])) & ~0x3F));
            memcpy(alignedBuffer, str, length);
            sceSifMCallRpc((sceSifMClientData *)&sdfPrintRpcClient.client, 0x7D1, 0, alignedBuffer, length, NULL, 0, NULL, NULL);
        }
    } else {
        scePrintf(sdfPrintStrFmt, str);
    }
}

s32 sdfPrintf(const char *str, ...) {
    char buffer[0x100];
    __builtin_va_list args;
    s32 length;
    __builtin_stdarg_start(args, str);
    length = vsprintf(buffer, str, args);
    sdfPuts(buffer);
    return length;
}

INCLUDE_SDATA(const s32, "sdf/sdfPrint", sdfPrintIopFlag);

INCLUDE_SDATA(const s32, "sdf/sdfPrint", sdfPrintStrFmt);

