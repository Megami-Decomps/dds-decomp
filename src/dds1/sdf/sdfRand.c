#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

extern s32 sdfCdTrayMode;

extern s32 sdfFileSemaId;

extern u8 sdfFileDefaultSrcType;

extern u8 sdfFileCmdDefaultDevice;

extern u8 sdfFileCmdThreadPrioCheckCounter;

typedef struct sdfHostPrefix_t {
    s8 b[7];
} sdfHostPrefix_t;

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

typedef struct sdfRandCB_t {
    u32 v[4];
} sdfRandCB_t;

typedef struct sdfClock_t {
    u8 pad00;
    u8 sec;
    u8 min;
    u8 hour;
    u8 day;
    u8 month;
    u16 year;
} sdfClock_t;

typedef struct sdfCdClock_t {
    u8 status;
    u8 second;
    u8 minute;
    u8 hour;
    u8 pad04;
    u8 day;
    u8 month;
    u8 year;
} sdfCdClock_t;

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

typedef union { f32 quaternion[4]; u16 monthDays[20]; } sdfMathConstants_t;

extern sdfMathConstants_t sdfMathConstants;

extern s8 sdfPrintIopFlag;

extern char sdfPrintStrFmt[];

extern char sdfFileCdromPrefix[];

extern sdfHostPrefix_t sdfFileHostPrefix[];

extern u8 sdfFileHddPrefix[];

extern char sdfFileCdromName[];

extern sdfFileCmdHandle_t *sdfFileCmdActiveHead;

extern sdfFileCmdHandle_t *sdfFileCmdFreeList;

extern s16 sdfFileCmdFreeCount;

extern s16 sdfFileCmdCount;

extern sdfFileCmdHandle_t *sdfFileCmdCacheHead;

extern sdfFileCmdHandle_t *sdfFileCmdCacheTail;

extern char sdfFileCmdCdromPrefix[];

extern char sdfFileCmdHostPrefix[];

extern char sdfFileCmdPfsPrefix[];

extern u16 sdfFileCmdDefaultCreateMode;

extern s32 sdfFileCmdThreadPrio;

extern u8 sdfDev9Ready;

extern sdfRandCB_t sdfDefaultRandCB;

extern s32 sceCdReadClock(sdfCdClock_t *);

extern u8 sdfCdClock;

/* DDS defines the clock cache in this unit. */
sdfClock_t sdfCdClockCache = {0};

extern s32 (*p2sdSmgBuf)(void);

s32 sdfGetCdClock(sdfClock_t *now);

s32 sdfBcdToDecimal(s32 aBcd);

/* Seed the generator state (the shared effect state when `state` is NULL); each word is the previous one rotated right 8 bits and xored with a constant. */
void sdfRandomize(sdfRandCB_t *aCB, u32 aSeed) {
    if (aCB == NULL) {
        aCB = &sdfDefaultRandCB;
    }
    aSeed ^= 0x0AED1A0C;
    aCB->v[0] = aSeed;
    aSeed = (aSeed << 24) | (aSeed >> 8);
    aCB->v[1] = aSeed;
    aSeed ^= 0xAA5A02FE;
    aSeed = (aSeed << 24) | (aSeed >> 8);
    aCB->v[2] = aSeed;
    aSeed ^= 0x11BE81C7;
    aSeed = (aSeed << 24) | (aSeed >> 8);
    aCB->v[3] = aSeed;
}

/* Seeds the generator at `state` from the current clock (time of day in seconds since an epoch). */
void sdfRandomizeByTime(sdfRandCB_t *aCB) {
    sdfClock_t now;
    s32 secondsSinceEpoch;
    sdfGetCdClock(&now);
    secondsSinceEpoch = now.year * 0x16E + now.day + sdfMathConstants.monthDays[now.month + 7] + 0xFFF4FF83;
    secondsSinceEpoch = secondsSinceEpoch * 0x18 + now.hour;
    secondsSinceEpoch = secondsSinceEpoch * 0x3C + now.min;
    sdfRandomize(aCB, secondsSinceEpoch * 0x3C + now.sec);
}

s32 sdfGetCdClock(sdfClock_t *now) {
    sdfCdClock_t clock;
    s32 result = 1;
    s32 year;
    if (sdfCdClock == 0) {
        sceCdReadClock(&clock);
        result = -1;
        if ((clock.status & 0x103) == 0) {
            sdfCdClockCache.sec = sdfBcdToDecimal(clock.second);
            sdfCdClockCache.min = sdfBcdToDecimal(clock.minute);
            sdfCdClockCache.hour = sdfBcdToDecimal(clock.hour);
            sdfCdClockCache.day = sdfBcdToDecimal(clock.day);
            sdfCdClockCache.month = sdfBcdToDecimal(clock.month);
            /* Retail reuses the month byte as the two-digit year input. */
            year = sdfBcdToDecimal(clock.month);
            result = 0;
            sdfCdClockCache.year = year + (year < 70 ? 2000 : 1900);
        }
        sdfCdClock = 20;
    }
    *now = sdfCdClockCache;
    return result;
}
