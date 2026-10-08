#ifndef FILE_PAC_H
#define FILE_PAC_H

#include "common.h"
#include "sdf_pac_packet.h"
#include "sdf_pac_state.h"
#include "sdf_pac_work.h"

struct FileNode;
struct PacBuf;
struct PacAlloc;

/* 0x70-byte file request: PAC state at 0x30, readiness gate at 0x68. */
typedef struct FilePacRequest {
    u8 kind;
    u8 state;
    u8 pad02[2];
    struct FileNode *next;
    char *name;
    u32 handle;
    s32 size;
    u8 pad14[4];
    void *callback;
    void *userData;
    u8 pad20[0x10];
    PacState packet;
    u16 readinessEnabled;
    u16 slot;
    u8 pad6C[4];
} FilePacRequest;

typedef char FilePacRequest_size_must_be_0x70[(sizeof(FilePacRequest) == 0x70) ? 1 : -1];
typedef char FilePacRequest_queue_must_be_at_0x60[
    ((u32)&((FilePacRequest *)0)->packet.queueHead == 0x60) ? 1 : -1];
#endif /* FILE_PAC_H */
