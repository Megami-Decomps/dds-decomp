#ifndef FILE_H
#define FILE_H

#include "common.h"

/* File manager job states, request entry size and transfer chunk. */
#define FILE_JOB_READY 3
#define FILE_JOB_TRANSFERRING 4
#define FILE_REQ_WORDS_PER_ENTRY 0x19
#define FILE_IO_MAX_CHUNK_BYTES 0x8000

struct FileNode;
struct FileRequest;
struct FileCbNode;

/* File-job buffer offsets are relative to the job unless allocation is retained. */
typedef struct FileJobBufferSlot {
    u32 offset;
    u32 size;
    void *allocation;
    u16 selector;
    u16 unkE;
} FileJobBufferSlot;

/* Separately allocated payload and its serialized 0x2C header. Queue entries
 * refer to this object through their address-valued id field. */
typedef struct FileJobPayload {
    u32 unk0;
    u16 type;
    u16 unk6;
    void *data;
    u16 option;             /* selects primary payload interpretation */
    u16 unkE;
    FileJobBufferSlot primary; /* selector at 0x1C describes the secondary payload */
    struct {
        u32 offset;
        u32 size;
        void *allocation;
    } secondary;            /* 0x20; no selector follows this descriptor */
} FileJobPayload;

typedef char FileJobPayload_size_must_be_0x2C[
    (sizeof(FileJobPayload) == 0x2C) ? 1 : -1];

/* Effect/file queue entries share this C0-byte record in both games. */
typedef struct FileJob {
    u32 unk0;
    u16 type;
    u16 unk6;
    void *data;
    u16 option;
    u16 unkE;
    FileJobBufferSlot slots[2];
    u8 unk30[0x10];
    f32 offset[4];    /* 0x40 */
    f32 quat[4];      /* 0x50 */
    f32 scale;        /* 0x60 */
    u32 color;        /* 0x64 */
    u32 xformFlags;   /* 0x68 */
    u8 unk6C[0x14];
    s32 unk80;
    u32 scaleFlags;   /* 0x84 */
    u8 unk88[8];
    u32 id;
    u32 sector;
    u32 flags;
    char name[0x10];  /* 0x9C: queue-entry resource name */
    struct FileJob *next;
    struct FileJob *prev;
    u8 padB4[0xC];
} FileJob;

typedef char FileJobBufferSlot_size_must_be_0x10[
    (sizeof(FileJobBufferSlot) == 0x10) ? 1 : -1];
typedef char FileJob_size_must_be_0xC0[
    (sizeof(FileJob) == 0xC0) ? 1 : -1];

/* One of the four device-read slots at FileManWork + 0x20. */
typedef struct FileManSlot {
    u32 value;
    struct FileRequest *request;
} FileManSlot;

typedef struct FileManWork {
    s32 sema;                 /* 0x00 */
    u8 currentSlot;           /* 0x04 */
    u8 nextSlot;              /* 0x05 */
    u8 activeSlots;           /* 0x06 */
    u8 freeSlots;             /* 0x07 */
    struct FileNode *head;    /* 0x08: queued requests, linked through +0x4 */
    struct FileNode *tail;    /* 0x0C */
    struct FileCbNode *done;  /* 0x10: completed callbacks */
    void *unk14;              /* 0x14 */
    u32 unk18;                /* 0x18 */
    u32 buffer;               /* 0x1C */
    FileManSlot slots[4];     /* 0x20 */
} FileManWork;

typedef char FileManWork_size_must_be_0x40[(sizeof(FileManWork) == 0x40) ? 1 : -1];

extern FileManWork fileManagerWork;

/* Queue a callback-kind request without a completion callback; return its work. */
void *fileQueueDefaultCallbackRequest(const char *requestName);

#endif /* FILE_H */
