#ifndef FILE_REQUEST_ENTRY_H
#define FILE_REQUEST_ENTRY_H

#include "common.h"
#include "file_request_poll_result.h"

/* Card state written when sdfMcFuncGetInfo1 completes, distinct from its poll result. */
typedef u8 FileReqStatus;
enum FileReqStatusValue {
    FILE_REQ_STATUS_NO_CARD = 0,
    FILE_REQ_STATUS_READY = 1,
    FILE_REQ_STATUS_UNFORMATTED = 2,
    FILE_REQ_STATUS_UNSUPPORTED_CARD = 3
};

/* Request-indexed memory-card state, one 0x64-byte entry per request. */
typedef struct FileReqEntry {
    u32 unk0;      /* 0x00 */
    u32 cardType;  /* 0x04: card-info RPC type output. */
    u32 sizeKiB;   /* 0x08: sdfMcGetFreeBytes converts this to bytes */
    u32 formatted; /* 0x0C: zero means unformatted. */
    u8 requestRetryCount; /* 0x10: reset on begin; retries request-mode 2 up to five times. */
    FileReqStatus status; /* 0x11: completed card-detection result */
    u8 slotMetadataDirty; /* 0x12 */
    s8 selectedSlot; /* 0x13 */
    u32 slotFlags[20]; /* 0x14: includes the twenty per-request slot words */
} FileReqEntry;

typedef char FileReqEntrySizeCheck[
    (sizeof(FileReqEntry) == 0x64) ? 1 : -1];
typedef char FileReqEntryRequestRetryCountOffsetCheck[
    ((u32)&((FileReqEntry *)0)->requestRetryCount == 0x10) ? 1 : -1];
typedef char FileReqEntrySlotFlagsOffsetCheck[
    ((u32)&((FileReqEntry *)0)->slotFlags == 0x14) ? 1 : -1];

/* The flat flag view aliases entry slotFlags at +0x14; the index remains a
 * request/slot word offset, and the request-table extent is not specified. */
extern FileReqEntry sdfMcCb[];
extern u32 fileRequestSlotFlags[];

void sdfMcRegetInfo(s32 request);
FileReqPollResult sdfMcFuncGetInfo1(void);
FileReqStatus sdfMcGetFileInfoReady(s32 request);
s32 sdfMcGetFreeBytes(s32 request);
u8 sdfMcGetStatus(s32 request);
void sdfMcClearCardStat(s32 request);
void sdfMcSetCardStat(s32 request);
s8 sdfMcGetSlotIndex(s32 request);
void sdfMcSetSlotIndex(s32 request, s32 selectedSlot);

#endif
