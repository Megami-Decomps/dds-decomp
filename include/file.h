#ifndef FILE_H
#define FILE_H

#include "common.h"

/* File manager job states, request entry size and transfer chunk. */
#define FILE_JOB_READY 3
#define FILE_JOB_TRANSFERRING 4
#define FILE_REQ_WORDS_PER_ENTRY 0x19
#define FILE_IO_MAX_CHUNK_BYTES 0x8000

struct FileNode;
struct FileQueue;
struct FileRequest;
struct FileCbNode;
struct SdfMemBlock;

/* File-job buffer offsets are relative to the job unless allocation is retained. */
typedef struct FileJobBufferSlot {
    u32 offset;
    u32 size;
    void *allocation;
    u16 selector;
    u16 unkE;
} FileJobBufferSlot;

/* Runtime payload buffers own a general-heap descriptor; queue image slots
 * keep their separate opaque allocation word in FileJobBufferSlot. */
typedef struct FileJobPayloadBufferSlot {
    u32 offset;
    u32 size;
    struct SdfMemBlock *allocation;
    u16 selector;
    u16 unkE;
} FileJobPayloadBufferSlot;

/* Separately allocated payload and its serialized 0x2C header. Queue entries
 * refer to this object through their address-valued id field. */
typedef struct FileJobPayload {
    u32 unk0;
    u16 type;
    u16 unk6;
    void *data;
    u16 option;             /* selects primary payload interpretation */
    u16 unkE;
    FileJobPayloadBufferSlot primary; /* selector at 0x1C describes the secondary payload */
    struct {
        u32 offset;
        u32 size;
        struct SdfMemBlock *allocation;
    } secondary;            /* 0x20; no selector follows this descriptor */
} FileJobPayload;

typedef char FileJobPayload_size_must_be_0x2C[
    (sizeof(FileJobPayload) == 0x2C) ? 1 : -1];

void *fileResolvePrimaryBuffer(FileJobPayload *job);
void *fileResolveSecondaryBuffer(FileJobPayload *job);
FileJobPayload *fileJobCreateFromJob(FileJobPayload *request);
FileJobPayload *fileJobCreateChild(FileJobPayload *request);
FileJobPayload *fileDuplicateJob(FileJobPayload *request);
FileJobPayload *fileJobCreateFromCommandState(const char *entry);
FileJobPayload *fileCreateJob(u16 type);
void fileJobDestroy(FileJobPayload *job);
void fileWriteToPfs(FileJobPayload *job, const char *filePath);
void fileJobWriteSerializedPayload(s32 fd, FileJobPayload *job);
void fileJobSetPrimaryData(FileJobPayload *job, const void *src, s32 size, u16 option);
void fileJobSetSecondaryData(FileJobPayload *job, const void *src, s32 size, u16 selector);
void fileJobCopyCommandIntoPrimaryData(FileJobPayload *job, const char *commandPath, u16 option);
void fileJobCopyCommandIntoSecondaryData(FileJobPayload *job, const char *commandPath, u16 selector);

struct FileQueue *fileCloneQueueEntries(struct FileQueue *source);
struct FileQueue *fileQueueClone(struct FileQueue *source);
void fileQueueDestroy(struct FileQueue *queue);
void fileQueueSetPosition(struct FileQueue *queue, const f32 position[4]);
void fileQueueSetRotation(struct FileQueue *queue, const f32 rotation[4]);
void fileQueueSetScale(struct FileQueue *queue, f32 scale);
void fileQueueUpdate(struct FileQueue *queue);

/* Queue flags describe payload sharing and secondary-buffer links. */
#define FILE_JOB_FLAG_SHARED_PAYLOAD 0x1
#define FILE_JOB_FLAG_SECTOR_FOLLOWER 0x2

/* Known update-policy bits; other bits remain uninterpreted. */
#define FILE_JOB_UPDATE_FLAG_APPLY_QUEUE_SCALE 0x1
#define FILE_JOB_UPDATE_FLAG_SKIP_FRAME_UPDATE 0x2

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
    s32 startFrame;   /* 0x80: earliest queue frame eligible for updates */
    u32 updateFlags;  /* 0x84 */
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
typedef char FileJobPayloadBufferSlot_size_must_be_0x10[
    (sizeof(FileJobPayloadBufferSlot) == 0x10) ? 1 : -1];
typedef char FileJob_size_must_be_0xC0[
    (sizeof(FileJob) == 0xC0) ? 1 : -1];

/* Runtime queue owner; serialized images retain the same 0x90-byte header. */
typedef struct FileQueue {
    f32 offset[4];
    f32 axis[4];
    u8 unk20[0x20];
    f32 position[4];  /* 0x40 */
    f32 quat[4];      /* 0x50 */
    f32 scale;        /* 0x60 */
    u32 color;        /* 0x64: modulation colour */
    u32 transformFlags; /* 0x68: bits 0x60 select the rotation branch */
    u8 pad6C[8];
    f32 scaleMultiplier; /* 0x74: multiplies queue scale during updates */
    u8 pad78[8];
    s32 count;        /* 0x80 */
    u32 updateFrame;  /* 0x84 */
    union {
        FileJob *last;   /* runtime linked-list tail */
        u32 entryOffset; /* relative first-entry offset in serialized images */
    };               /* 0x88 */
    FileJob *first;   /* 0x8C: runtime traversal start */
} FileQueue;

typedef char FileQueue_size_must_be_0x90[
    (sizeof(FileQueue) == 0x90) ? 1 : -1];
typedef char FileQueue_scaleMultiplier_offset_must_be_0x74[
    ((u32)&((FileQueue *)0)->scaleMultiplier == 0x74) ? 1 : -1];
typedef char FileQueue_first_offset_must_be_0x8C[
    ((u32)&((FileQueue *)0)->first == 0x8C) ? 1 : -1];

FileQueue *fileQueueCreate(void);
void fileQueueSetColor(FileQueue *queue, u32 color);
FileQueue *fileQueueCreateFromCommandState(const char *entry);
void fileQueueCopyRotationFromSource(FileQueue *queue, f32 matrix[4][4]);
void fileQueueSaveVersionedImage(FileQueue *queue, const char *filePath);
void fileQueueSaveImage(FileQueue *queue, const char *filePath);
FileJob *fileQueueGetAt(FileQueue *queue, s32 index);
FileJob *fileAppendJobFromCommandPath(FileQueue *queue, const char *commandPath);
FileJob *fileJobDuplicateAfter(FileQueue *queue, FileJob *source);
void fileJobCopyHeader(FileJob *destination, FileJob *source);
void fileQueueRemoveAndDestroyJob(FileQueue *queue, FileJob *job);
void fileQueueDetachSectorFollower(FileQueue *queue, FileJob *job);
void fileQueueLinkJobToSectorLeader(FileQueue *queue, FileJob *job, FileJob *leader);

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
    u8 *buffer;               /* 0x1C: retained base of the four device-read slots */
    FileManSlot slots[4];     /* 0x20 */
} FileManWork;

typedef char FileManWork_size_must_be_0x40[(sizeof(FileManWork) == 0x40) ? 1 : -1];

extern FileManWork fileManagerWork;

/* Queue a callback-kind request without a completion callback; return its work. */
struct FileRequest *fileQueueDefaultCallbackRequest(const char *requestName);

#endif /* FILE_H */
