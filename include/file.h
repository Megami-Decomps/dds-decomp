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

/* Reentrancy guard and adjacent reserved word retained in the retail GP window. */
typedef struct FileManGuardState {
    s32 active;
    s32 reserved;
} FileManGuardState;

typedef char FileManGuardState_size_must_be_8[(sizeof(FileManGuardState) == 8) ? 1 : -1];

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

#endif /* FILE_H */
