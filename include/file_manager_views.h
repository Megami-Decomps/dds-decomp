#ifndef FILE_MANAGER_VIEWS_H
#define FILE_MANAGER_VIEWS_H

#include "common.h"

struct FileNode;

/* Queue-entry fields written by fileManQueueNamedRequest; linkage is supplied
 * by its caller and follows the title-specific FileNode prefix. */
typedef struct FileQueueEntry {
    u8 kind;
    u8 pad01[3];
    struct FileNode *next;
    char *name;
    u8 pad0C[0xC];
    void *callback;
    void *userData;
} FileQueueEntry;

/* Partial work view used by the list helpers and resource accessors. The word
 * at +0x10 and surrounding spans remain opaque. */
typedef struct FileWork {
    u8 unk0[0x10];
    u32 unk10;
    u32 size;
    struct FileNode *head;
    u8 unk1C[4];
    u32 resourceHandle;
    u32 loadedDataAddress;
} FileWork;

/* Request state used by readiness and read-slot clients; padding and the
 * allocation's total extent remain unclaimed. */
typedef struct FileRequest {
    u8 kind;
    u8 state;
    u8 pad02[0xA];
    u32 handle;
    s32 size;
    u8 pad14[0x54];
    u16 readinessEnabled;
    u16 slot;
} FileRequest;

/* Cleanup overlay used when a queued request reaches completion. */
typedef struct FileCleanup {
    u8 kind;
    u8 state;
    u8 pad02[6];
    void *requestNameCopy;
    u32 handle;
} FileCleanup;

/* Complete 0x30 value-pair request allocated by fileWindowSlotCreate. */
typedef struct FileWindowSlot {
    u8 pad00[0x10];
    s32 secondValueCopy;
    s32 secondValue;
    u8 pad18[0xC];
    s32 firstValue;
    s32 firstValueCopy;
    u8 pad2C[4];
} FileWindowSlot;

typedef char FileQueueEntry_next_offset_check[
    ((u32)&((FileQueueEntry *)0)->next == 0x04) ? 1 : -1];
typedef char FileQueueEntry_name_offset_check[
    ((u32)&((FileQueueEntry *)0)->name == 0x08) ? 1 : -1];
typedef char FileQueueEntry_callback_offset_check[
    ((u32)&((FileQueueEntry *)0)->callback == 0x18) ? 1 : -1];
typedef char FileQueueEntry_userData_offset_check[
    ((u32)&((FileQueueEntry *)0)->userData == 0x1C) ? 1 : -1];

typedef char FileWork_size_offset_check[
    ((u32)&((FileWork *)0)->size == 0x14) ? 1 : -1];
typedef char FileWork_head_offset_check[
    ((u32)&((FileWork *)0)->head == 0x18) ? 1 : -1];
typedef char FileWork_resourceHandle_offset_check[
    ((u32)&((FileWork *)0)->resourceHandle == 0x20) ? 1 : -1];
typedef char FileWork_loadedDataAddress_offset_check[
    ((u32)&((FileWork *)0)->loadedDataAddress == 0x24) ? 1 : -1];

typedef char FileRequest_handle_offset_check[
    ((u32)&((FileRequest *)0)->handle == 0x0C) ? 1 : -1];
typedef char FileRequest_size_offset_check[
    ((u32)&((FileRequest *)0)->size == 0x10) ? 1 : -1];
typedef char FileRequest_readinessEnabled_offset_check[
    ((u32)&((FileRequest *)0)->readinessEnabled == 0x68) ? 1 : -1];
typedef char FileRequest_slot_offset_check[
    ((u32)&((FileRequest *)0)->slot == 0x6A) ? 1 : -1];

typedef char FileCleanup_requestNameCopy_offset_check[
    ((u32)&((FileCleanup *)0)->requestNameCopy == 0x08) ? 1 : -1];
typedef char FileCleanup_handle_offset_check[
    ((u32)&((FileCleanup *)0)->handle == 0x0C) ? 1 : -1];

typedef char FileWindowSlot_size_must_be_0x30[
    (sizeof(FileWindowSlot) == 0x30) ? 1 : -1];
typedef char FileWindowSlot_secondValue_offset_check[
    ((u32)&((FileWindowSlot *)0)->secondValue == 0x14) ? 1 : -1];
typedef char FileWindowSlot_firstValue_offset_check[
    ((u32)&((FileWindowSlot *)0)->firstValue == 0x24) ? 1 : -1];
typedef char FileWindowSlot_firstValueCopy_offset_check[
    ((u32)&((FileWindowSlot *)0)->firstValueCopy == 0x28) ? 1 : -1];

#endif /* FILE_MANAGER_VIEWS_H */
